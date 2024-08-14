pipeline {
    agent {
        label "Node3_4CPU"
    }
    tools {
        maven "maven 3.9.6"
        jdk "Java17"
    }

    parameters {
        booleanParam(name: "RELEASE",
                description: "Build a release from current commit.",
                defaultValue: false)
    }

    stages {
        stage ("Build and Deploy SNAPSHOT") {
            when {
                expression { ! params.RELEASE }
            }
            steps {
                sh "mvn -s /home/jenkins/ccs/maven/ccs-settings.xml -U clean install deploy:deploy site:site site:deploy" 
            }
        }

        stage("Release") {
            when {
                expression { params.RELEASE }
            }   
            steps {
                sh "mvn -s /home/jenkins/ccs/maven/ccs-settings.xml -U -Dresume=false clean release:prepare release:perform"
            }
        }
    }

    post {
        always {
            sh "/home/jenkins/ccs/scripts/updateJiraVersions.sh"
        }
    }

}
