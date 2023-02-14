package org.lsst.ccs.daq.guider;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.annotation.JsonDeserialize;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import com.fasterxml.jackson.databind.deser.std.StdDeserializer;
import com.fasterxml.jackson.databind.ser.std.StdSerializer;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import org.lsst.ccs.daq.ims.DAQException;
import org.lsst.ccs.utilities.location.LocationSet;

/**
 * Implementation of ROISpec for LSE-71 initGuider command.
 *
 * @author tonyj
 */
@JsonSerialize(using = ROISpec.ROISpecSerializer.class)
@JsonDeserialize(using = ROISpec.ROISpecDeserializer.class)
public class ROISpec {

    private final ROICommon common;
    private final List<ROILocation> locations;

    ROISpec(ROICommon common, List<ROILocation> locations) {
        this.common = common;
        this.locations = locations;
    }

    public ROICommon getCommon() {
        return common;
    }

    public List<ROILocation> getLocations() {
        return locations;
    }

    public static ROISpec parse(String roiSpec) throws IllegalArgumentException {
        try {
            ObjectMapper om = new ObjectMapper();
            return om.readValue(roiSpec, ROISpec.class);
        } catch (JsonProcessingException x) {
            throw new IllegalArgumentException("Invalid ROISpec " + roiSpec, x);
        }
    }

    public void sanityCheck(LocationSet configuredLocations) throws DAQException {
        for (ROILocation l : locations) {
            if (!configuredLocations.contains(l.getLocation().getRebLocation())) {
                throw new IllegalArgumentException("Location "+l+" not configured as a guider");
            }
        }
    }
       
    @Override
    public String toString() {
        try {
            ObjectMapper om = new ObjectMapper();
            return om.writeValueAsString(this);
        } catch (JsonProcessingException ex) {
            throw new RuntimeException("Can't happen!!!", ex);
        }
    }

    static class ROISpecSerializer extends StdSerializer<ROISpec> {

        public ROISpecSerializer() {
            super(ROISpec.class, false);
        }

        @Override
        public void serialize(ROISpec t, JsonGenerator jg, SerializerProvider sp) throws IOException {
            jg.writeStartObject();
            jg.writeObjectField("common", t.getCommon());
            for (ROILocation roiLocation : t.getLocations()) {
                Map<String, Integer> map = new LinkedHashMap<>();
                map.put("segment", roiLocation.getSegment());
                map.put("startRow", roiLocation.getStartRow());
                map.put("startCol", roiLocation.getStartCol());
                SensorLocation sensorLocation = roiLocation.getLocation();
                jg.writeObjectField(sensorLocation.toString(), map);
            }
            jg.writeEndObject();

        }
    }

    static class ROISpecDeserializer extends StdDeserializer<ROISpec> {

        public ROISpecDeserializer() {
            super(ROISpec.class);
        }

        @Override
        public ROISpec deserialize(JsonParser jp, DeserializationContext dc) throws IOException, JacksonException {
            ObjectMapper om = new ObjectMapper();
            JsonNode node = jp.getCodec().readTree(jp);
            JsonNode commonNode = node.get("common");
            ROICommon common = om.treeToValue(commonNode, ROICommon.class);
            List<ROILocation> locations = new ArrayList<>();
            Iterator<Map.Entry<String, JsonNode>> fields = node.fields();
            while (fields.hasNext()) {
                Map.Entry<String, JsonNode> next = fields.next();
                if (next.getKey().equals("common")) {
                    continue;
                }
                SensorLocation sensorLocation = SensorLocation.of(next.getKey());
                Map<String, Integer> map = om.treeToValue(next.getValue(), Map.class);
                int segment = map.get("segment");
                int startRow = map.get("startRow");
                int startCol = map.get("startCol");
                locations.add(new ROILocation(sensorLocation, segment, startRow, startCol));
            }

            return new ROISpec(common, locations);
        }

    }
}
