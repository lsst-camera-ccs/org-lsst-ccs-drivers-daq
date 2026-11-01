# Store.cpp review findings (pre-existing, not yet fixed)

Source: review of `src/main/c/Store.cpp` on branch `LSSTCCSDRIVER-489`
(DAQ R5-v13.14 upgrade). These are **pre-existing** issues, not regressions from
the upgrade. Recorded 2026-10-31 to return to later; not being fixed now (keeping
the current change minimal).

## 1. `writeRegisters` releases the wrong array handle — BUG
**JIRA: [LSSTCCSDRIVER-494](https://jira.slac.stanford.edu/browse/LSSTCCSDRIVER-494)** (fix in PR #17)

`src/main/c/Store.cpp` ~line 1048-1049:
```cpp
env->ReleaseIntArrayElements(addresses, regArray, JNI_ABORT);
env->ReleaseIntArrayElements(addresses, regValues, JNI_ABORT);   // should be `values`
```
`regValues` was obtained from `values`, not `addresses`. Releasing against the
wrong array is undefined behavior (JNI requires the same array the elements came
from).

## 2. `openSourceChannel` — null check can't fire, returns dangling pointer — BUG
**JIRA: [LSSTCCSDRIVER-495](https://jira.slac.stanford.edu/browse/LSSTCCSDRIVER-495)**

~line 787-794:
```cpp
Source* source = new Source(id_, element, *store_);
if (!source) {            // never true: `new` throws, never returns NULL
    ... throwDAQException(...);
    delete source;        // if entered, deletes then falls through
}
return (jlong) source;    // returns freed pointer on the error branch
```
Intent was to validate the Source via its `operator bool` (`if (!*source)`), as
every other function here does with a stack `Source`/`Image`. As written a failed
source is never detected, and the error branch returns a freed pointer.

## 3. `startGuider` / `validateGuider` — unbounded `nlocs` vs fixed buffer — BUG
~line 406-417 and 444-454:
```cpp
GDS::RoiLocation locs[MAX_GUIDER_LOCATIONS];   // 8
int nlocs = env->GetArrayLength(roiData)/5;
for (int i=0; i<nlocs; i++) { locs[i] = ... }  // stack overflow if >8 locations
```
A Java caller passing >8 ROI locations overruns the stack buffer. Needs a guard
against `MAX_GUIDER_LOCATIONS`.

## 4. String leak on constructor-exception path in attach functions — minor
`attachStore` (~324-331), `attachGuider` (~353-360), `attachClient` (~974-981):
`ReleaseStringUTFChars` for `partition_name` is only on the success path before
`return`. If the constructor throws `DSM::Exception`, the string is leaked.

## 5. Post-exception continuation — fragile pattern
Several functions call `throwDAQException(...)` (sets a pending JNI exception) then
keep issuing JNI calls / fall through (e.g. `addImageToFolder`, `triggerImage`,
`findFolder` throws then still does `ReleaseStringUTFChars` and `return 1`).
`Release*` is safe with a pending exception, but the broader pattern is fragile and
relies on Java-side checks.

## Minor
- VLA `jint register_values[size]` (~line 144) is non-standard C++.
- signed/unsigned mismatches in register loops (`uint32_t i < jint size`).
