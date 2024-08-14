def _user="appuser"
def _home="/home/" + _user

pipeline {
    agent {

        docker { 
            image 'ts-dockerhub.lsst.org/robotsal:alma9'
            args '-w ' + _home + ' 
            label "Node3_4CPU"
        }
//        label "Node2_8CPU"
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
                sh "printenv"
                sh "which mvn"
                sh "echo $PATH"
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

            //Email Notification
            step([$class: 'Mailer',
                notifyEveryUnstableBuild: true,
                recipients: emailextrecipients("mxturri@gmail.com",culprits(),requestor()),
                sendToIndividuals: true])
        }
    }

}
