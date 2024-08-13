pipeline {
    agent {
        label 'Node1_4CPU||Node2_8CPU||Node3_4CPU'
    }
    tools {
        maven 'maven 3.9.6'
        jdk 'Java17'
    }
    stages {
        stage ('Initialize') {
            steps {
                sh '''
                    echo "PATH = ${PATH}"
                    echo "M2_HOME = ${M2_HOME}"
                '''
            }
        }

        stage ('Build') {
            steps {
                sh 'mvn -s /home/jenkins/ccs/maven/ccs-settings.xml -U clean install deploy:deploy site:site site:deploy' 
            }
        }
    }
}