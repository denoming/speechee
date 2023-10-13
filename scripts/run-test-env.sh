#!/usr/bin/env bash

set -e

DOCKER_IMAGE_NAME=my/jarvis-dev-image:speechee

USER_NAME=${USER}
USER_UID=$(id -u)
USER_GID=$(id -g)

PROJECT_DIR=$(dirname "$(dirname "$(realpath -s $0)")")

command -v docker > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "Docker is not installed"
    exit 1
fi

build_image() {
  BUILD_CMD="docker build --tag ${DOCKER_IMAGE_NAME} \
  --build-arg UNAME=${USER_NAME} \
  --build-arg UID=${USER_UID} \
  --build-arg GID=${USER_GID} \
  --build-arg PULSE_SERVER="unix:${XDG_RUNTIME_DIR}/pulse/native" \
  --file ${PROJECT_DIR}/Dockerfile ${PROJECT_DIR}
  "

  if [ -z "$(docker images -q ${DOCKER_IMAGE_NAME})" ]; then
    echo -e "Building <${DOCKER_IMAGE_NAME}> image"
    echo "${BUILD_CMD}"
    ${BUILD_CMD}
  fi
}

run_image() {
  RUN_CMD=(docker run -it \
  --hostname "${USER_NAME}" \
  -p 8080:8080 \
  --rm \
  --user="${USER_UID}:${USER_GID}" \
  --volume="${PROJECT_DIR}:${PROJECT_DIR}" \
  --volume="${XDG_RUNTIME_DIR}/pulse:${XDG_RUNTIME_DIR}/pulse" \
  --volume="$HOME/.local/share/speechee:$HOME/.local/share/speechee" \
  --network=bridge \
  --workdir="${PROJECT_DIR}" \
  --env SPEECHEE_CONFIG="$HOME/.local/share/speechee/speechee.cfg" \
  --env PULSE_SERVER="unix:${XDG_RUNTIME_DIR}/pulse/native" \
  --env GOOGLE_APPLICATION_CREDENTIALS="$HOME/.local/share/speechee/google-cloud-credentials.json" \
  --entrypoint="/usr/sbin/entrypoint.sh" \
  "${DOCKER_IMAGE_NAME}")

  if [ -n "$(docker images -q ${DOCKER_IMAGE_NAME})" ]; then
    "${RUN_CMD[@]}"
  else
    echo "Docker image <${DOCKER_IMAGE_NAME}> is absent"
  fi
}

build_image
run_image
