docker rm -f github-runner

docker run -d \
  --name=github-runner \
  -e TZ=Europe/Athens \
  --restart unless-stopped \
  github:latest
