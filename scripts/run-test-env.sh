#!/usr/bin/env bash

set -e

DOCKER_IMAGE_NAME=my/speechee

USER_NAME=${USER}
USER_UID=$(id -u)
USER_GID=$(id -g)
SERVICE_PORT=2350

PROJECT_DIR=$(dirname "$(dirname "$(realpath -s $0)")")
CONFIG_DIR="$HOME/.local/share/speechee"
BUILD_DIR="${PROJECT_DIR}/build-debug-docker"

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
  --device /dev/snd \
  --hostname "${USER_NAME}" \
  -p "${SERVICE_PORT}:${SERVICE_PORT}" \
  --rm \
  --user="${USER_UID}:${USER_GID}" \
  --volume="${HOME}/.ssh:${HOME}/.ssh" \
  --volume="${PROJECT_DIR}:${PROJECT_DIR}" \
  --volume="${XDG_RUNTIME_DIR}/pulse:${XDG_RUNTIME_DIR}/pulse" \
  --volume="${CONFIG_DIR}:${CONFIG_DIR}" \
  --network=bridge \
  --workdir="${PROJECT_DIR}" \
  # Path to config file
  --env SPEECHEE_CONFIG="${CONFIG_DIR}/speechee.cfg" \
  # Path to Google TTS cloud access configuration
  --env GOOGLE_APPLICATION_CREDENTIALS="${CONFIG_DIR}/speechee-cloud-access.json" \
  # Specify path to pulse socket file
  --env PULSE_SERVER="unix:${XDG_RUNTIME_DIR}/pulse/native" \
  # Configure paths for gstreamer plugin scanner
  --env GST_PLUGIN_SCANNER="${BUILD_DIR}/vcpkg_installed/x64-linux-dynamic/tools/gstreamer/gst-plugin-scanner" \
  --env GST_PLUGIN_PATH="${BUILD_DIR}/vcpkg_installed/x64-linux-dynamic/debug/plugins/gstreamer" \
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
