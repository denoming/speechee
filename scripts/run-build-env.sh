#!/usr/bin/env bash

set -e

PROJECT_ROOT=$(dirname "$(dirname "$(realpath -s $0)")")

# Handle arguments
PLATFORM_ARCH="${1:-arm64}"
PLATFORM_VARIANT="${2:-v8}"
USER_NAME="${3:-$(whoami)}"
ONNX_VERSION="${4:-1.20.1}"
GSLL_VERSION="${5:-0.42.0}"

# Define variables
ONNX_URL="https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_VERSION}/onnxruntime-linux-aarch64-${ONNX_VERSION}.tgz"
GSLL_URL="https://github.com/gsl-lite/gsl-lite/archive/refs/tags/v${GSLL_VERSION}.tar.gz"
VOXER_URL1=https://github.com/denoming/voxer/releases/download/v0.2.5/libvoxer_0.2.5_arm64.deb
VOXER_URL2=https://github.com/denoming/voxer/releases/download/v0.2.5/libvoxer-dev_0.2.5_arm64.deb
JARVISTO_URL1=https://github.com/denoming/jarvisto/releases/download/v0.3.7/libjarvisto_0.3.7_arm64.deb
JARVISTO_URL2=https://github.com/denoming/jarvisto/releases/download/v0.3.7/libjarvisto-dev_0.3.7_arm64.deb
USER_UID="$(id ${USER_NAME} -u)"
USER_GID="$(id ${USER_NAME} -g)"
PLATFORM="${PLATFORM_ARCH}${PLATFORM_VARIANT}"
IMAGE_NAME="my/speechee:${PLATFORM_ARCH}${PLATFORM_VARIANT}"
CONFIG_DIR="$HOME/.local/share/speechee"

echo "=============================="
echo "        Platform: ${PLATFORM}"
echo "           Image: ${IMAGE_NAME}"
echo "        Username: ${USER_NAME}"
echo "         User ID: ${USER_UID}"
echo "        Group ID: ${USER_GID}"
echo "=============================="

command -v docker > /dev/null
if [ $? != 0 ]; then
    echo -e "Error:"
    echo -e "Docker is not installed"
    exit 1
fi

build_image() {
  BUILD_CMD=(docker build \
  --platform "linux/${PLATFORM_ARCH}/${PLATFORM_VARIANT}" \
  --tag "${IMAGE_NAME}" \
  --build-arg "PLATFORM=${PLATFORM}" \
  --build-arg "USER_NAME=${USER_NAME}" \
  --build-arg "USER_UID=${USER_UID}" \
  --build-arg "USER_GID=${USER_GID}" \
  --build-arg "VOXER_URL1=${VOXER_URL1}" \
  --build-arg "VOXER_URL2=${VOXER_URL2}" \
  --build-arg "JARVISTO_URL1=${JARVISTO_URL1}" \
  --build-arg "JARVISTO_URL2=${JARVISTO_URL2}" \
  --build-arg "ONNX_URL=${ONNX_URL}" \
  --build-arg "GSLL_URL=${GSLL_URL}" \
  --build-arg PULSE_SERVER="unix:${XDG_RUNTIME_DIR}/pulse/native" \
  --file "${PROJECT_ROOT}/Dockerfile"
  "${PROJECT_ROOT}")

  if [ -z "$(docker images -q ${IMAGE_NAME})" ]; then
    echo -e "Building <${IMAGE_NAME}> image"
    echo "${BUILD_CMD[@]}"
    "${BUILD_CMD[@]}"
  fi
}

run_image() {
  RUN_CMD=(docker run -it \
  --platform "linux/${PLATFORM_ARCH}/${PLATFORM_VARIANT}" \
  --device /dev/snd \
  --rm \
  --user "${USER_UID}:${USER_GID}" \
  --volume "${HOME}/.ssh:${HOME}/.ssh" \
  --volume "${PROJECT_ROOT}:${PROJECT_ROOT}" \
  --volume "${XDG_RUNTIME_DIR}/pulse:${XDG_RUNTIME_DIR}/pulse" \
  --volume "${CONFIG_DIR}:${CONFIG_DIR}" \
  --network "host" \
  --workdir "${PROJECT_ROOT}" \
  --env SPEECHEE_CONFIG="${CONFIG_DIR}/speechee.cfg" \
  --env GOOGLE_APPLICATION_CREDENTIALS="${CONFIG_DIR}/speechee-cloud-access.json" \
  --env PULSE_SERVER="unix:${XDG_RUNTIME_DIR}/pulse/native" \
  --entrypoint="/usr/sbin/entrypoint.sh" \
  "${IMAGE_NAME}")

  if [ -n "$(docker images -q ${IMAGE_NAME})" ]; then
    "${RUN_CMD[@]}"
  else
    echo "Docker image <${IMAGE_NAME}> is absent"
  fi
}

build_image
run_image
