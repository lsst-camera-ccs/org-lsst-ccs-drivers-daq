pipeline {
    environment {
        CCS_NEXUS_DEPLOYMENT_USR = credentials('CCS_NEXUS_DEPLOYMENT_USR')
        CCS_NEXUS_DEPLOYMENT_PSW = credentials('CCS_NEXUS_DEPLOYMENT_PSW')
        JAVA_HOME = '/usr/lib/jvm/java-17-zulu-openjdk-jdk'
        GIT_CREDENTIALS = credentials('89815f34-ac5c-4fbb-9c7c-3850d4a88750')
        GIT_UERNAME = '${GIT_CREDENTIALS_USR}'
        GIT_PASSWORD = '${GIT_CREDENTIALS_PSW}'
    }
    agent {
        docker { 
            image 'ts-dockerhub.lsst.org/robotsal:latest'
            args '-v /home/jenkins/ccs:/home/saluser/ccs'
            label "Node3_4CPU||Node1_4CPU||Node2_8CPU"
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
        stage ("Configure Environment") {
            steps {
                script {
                   env.GIT_USERNAME = env.GIT_CREDENTIALS_USR
                   env.GIT_PASSWORD = env.GIT_CREDENTIALS_PSW
                }                
            }
        }

        stage ("Build and Deploy SNAPSHOT") {
            when {
                expression { ! params.RELEASE }
            }
            steps {
                sh "echo WE GO HERE"
                sh "echo test GIT CREDENTIALS: ${env.GIT_USERNAME} ${env.GIT_PASSWORD} ${env.GIT_CREDENTIALS_USR}"
                sh "/opt/maven/bin/mvn -version"
                sh "/opt/maven/bin/mvn -s /home/saluser/ccs/maven/pipeline-settings.xml -U clean install deploy:deploy site:site site:deploy" 
            }
        }

        stage("Release") {
            when {
                expression { params.RELEASE }
            }   
            steps {

                sh "echo WE GO HERE"
                sh "echo test GIT CREDENTIALS: ${env.GIT_USERNAME} ${env.GIT_PASSWORD} ${env.GIT_CREDENTIALS_USR}"
                sh "/opt/maven/bin/mvn -s /home/saluser/ccs/maven/pipeline-settings.xml -U -Dresume=false clean release:prepare release:perform"
            }
        }
    }

    post {
        always {
            sh "/home/saluser/ccs/scripts/updateJiraVersions.sh"

            //Email Notification
            step([$class: 'Mailer',
                notifyEveryUnstableBuild: true,
                recipients: "mxturri@gmail.com",
                sendToIndividuals: true])
        }
    }

}
