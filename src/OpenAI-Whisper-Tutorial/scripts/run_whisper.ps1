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

[CmdletBinding()]
param (
    [Parameter()]
    [String]
    $RootPath
)

Get-ChildItem -Path $RootPath -Recurse -File |
    Where-Object { $_.Extension -in @(".mp3", ".mp4", ".wav", ".mov")} |
    ForEach-Object {
        Write-Host "whisper.exe --language=English -f srt -o $($_.DirectoryName) $($_.FullName)"
        whisper.exe --language=English -f srt -o $_.DirectoryName $_.FullName
    }
