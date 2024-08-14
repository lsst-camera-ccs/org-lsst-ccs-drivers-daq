pipeline {
    environment {
        AWS_ACCESS_KEY_ID = credentials('AWS_ACCESS_KEY_ID')
        AWS_SECRET_ACCESS_KEY = credentials('AWS_SECRET_ACCESS_KEY')
        NEXUS_USER = credentials('nexus_username')
        NEXUS_PASS = credentials('nexus_password')
    }
    agent {
        docker { 
            image 'ts-dockerhub.lsst.org/robotsal:latest'
            args '-w ' + _home + ' -e AWS_ACCESS_KEY_ID=$aws_key_id -e AWS_SECRET_ACCESS_KEY=$aws_secret_key ' + 
            '-e NEXUS_USER=$nexus_user -e NEXUS_PASS=$nexus_pass ' +
            '-e LSST_DDS_PARTITION_PREFIX=citest -v ${WORKSPACE}:' + _home + '/trunk'
            label "Node3_4CPU"
        }
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
