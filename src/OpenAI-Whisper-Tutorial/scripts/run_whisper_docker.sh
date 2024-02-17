#/bin/bash

# Copyright 2024 Ashley R. Thomas
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# For OpenAI Whisper tutorial at https://www.youtube.com/watch?v=JpAebX7d9Q4

# This script is for use in running a docker image "our-whisper" as created
# in the tutorial using the rocm/pytorch image.

# In the tutorial, this script was named "run_whisper.sh" but was since renamed
# to "run_whisper_docker.sh" for clarity.

EXTENSIONS=("mp4" "mp3" "wav" "mov")
EXTENSIONS_REGEX=".*\.\("$(printf "\|%s" "${EXTENSIONS[@]}" | sed 's/^\\|//')"\)$"

ROOT_DIR=$1
if [ -z "$ROOT_DIR" ]; then
	echo "$(basename $0) <root_dir>"
	echo "Transcribe files within <root_dir> which have the following extensions: ${EXTENSIONS[@]}"
	exit 1
fi

trap "echo 'trap exit now.';exit 1;" SIGINT SIGTERM

find $ROOT_DIR -type f -iregex $EXTENSIONS_REGEX -print0 | sort -z |
	while IFS= read -r -d '' FILENAME; do
		HOST_DIRNAME=$(dirname "$FILENAME")
		BASENAME=$(basename "$FILENAME")
		CONTAINER_FILENAME="/shared/$BASENAME"
		echo "----------------------------------------------------------------------"
		echo "This script is PID=$$"
		echo "Processing $FILENAME..."
		echo "HOST_DIRNAME=$HOST_DIRNAME"
		echo "CONTAINER_FILENAME=$CONTAINER_FILENAME"
		docker run --rm -e PYTHONUNBUFFERED=1 --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --device=/dev/kfd --device=/dev/dri --group-add video --ipc=host --shm-size 16G -v "$HOST_DIRNAME":/shared our-whisper whisper --language English -o /shared -f srt --model small "$CONTAINER_FILENAME" || break;
	done
echo "Exiting"
