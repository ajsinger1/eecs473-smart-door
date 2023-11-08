#!/bin/bash
set -euxo pipefail
IMAGE_NAME=smart-handle-lambdas
LAMBDA_NAMES=("get-lock-state" "get-mode" "get-users" "new-user" "update-lock-state" "update-mode" "update-user-info" "update-user-state")

#this line is to authenticate with docker. needed for docker push
aws ecr get-login-password --region us-east-2 | docker login --username AWS --password-stdin 879798753360.dkr.ecr.us-east-2.amazonaws.com

# build, tag, push
docker build --platform linux/amd64 -t ${IMAGE_NAME}:test .

docker tag ${IMAGE_NAME}:test 879798753360.dkr.ecr.us-east-2.amazonaws.com/${IMAGE_NAME}:latest

docker push 879798753360.dkr.ecr.us-east-2.amazonaws.com/${IMAGE_NAME}:latest

#IMPORTANT. Lambda needs to be reconfigured to use the image after each new image push.

for LAMBDA_NAME in "${LAMBDA_NAMES[@]}"; do
    aws lambda update-function-code --function-name ${LAMBDA_NAME} --image-uri 879798753360.dkr.ecr.us-east-2.amazonaws.com/${IMAGE_NAME}:latest >/dev/null
done

#Make sure to delete old images

#how to create a new ecr to push other lambdas to
# aws ecr create-repository --repository-name smart-handle-lambdas --region us-east-2 --image-scanning-configuration scanOnPush=true --image-tag-mutability MUTABLE
