#!/bin/bash
#this line is to authenticate with docker. needed for docker push
aws ecr get-login-password --region us-east-2 | docker login --username AWS --password-stdin 879798753360.dkr.ecr.us-east-2.amazonaws.com

# build, tag, push
docker build --platform linux/amd64 -t new-user-lambda:test .

docker tag new-user-lambda:test 879798753360.dkr.ecr.us-east-2.amazonaws.com/new-user-lambda:latest

docker push 879798753360.dkr.ecr.us-east-2.amazonaws.com/new-user-lambda:latest

#IMPORTANT. Lambda needs to be reconfigured to use the image after each new image push.
#Make sure to delete old images

#how to create a new ecr to push other lambdas to
#aws ecr create-repository --repository-name <REPO_NAME> --region us-east-2 --image-scanning-configuration scanOnPush=true --image-tag-mutability MUTABLE
