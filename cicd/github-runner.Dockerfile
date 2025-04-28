FROM ubuntu:24.04

RUN apt-get update -y
RUN apt-get install -y build-essential gcc g++ make git vim cmake wget file jq curl cpplint cppcheck libopus-dev
RUN rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash pi
RUN echo "pi:123 | chpasswd"
#COPY .ssh /home/pi/.ssh
RUN chown -R pi:pi /home/pi/

ENV DPP_VER=10.1.2
ENV DPP_URL=https://github.com/brainboxdotcc/DPP/releases/download/v${DPP_VER}/libdpp-${DPP_VER}-linux-x64.deb
ENV RUNNER_VER=2.323.0
ENV RUNNER_URL=https://github.com/actions/runner/releases/download/v${RUNNER_VER}/actions-runner-linux-x64-${RUNNER_VER}.tar.gz
ENV PAT_TOKEN=

WORKDIR /tmp
RUN wget -O dpp.deb ${DPP_URL}
RUN apt install -y ./dpp.deb

USER pi
WORKDIR /home/pi

# GITHUB RUNNER
# Create a folder
RUN mkdir /home/pi/actions-runner
WORKDIR /home/pi/actions-runner
#RUN curl -o actions-runner.tar.gz ${RUNNER_URL}
RUN wget -O actions-runner.tar.gz ${RUNNER_URL}
RUN tar xzf ./actions-runner.tar.gz

# Create the runner and start the configuration experience
RUN REG_TOKEN=`curl -s -X POST \
                    -H "Authorization: token ${PAT_TOKEN}" \
                    -H "Accept: application/vnd.github+json" \
                    https://api.github.com/repos/mantzoun/luxbracer/actions/runners/registration-token| jq -r .token` \
    && ./config.sh --url https://github.com/mantzoun/luxbracer \
                --token ${REG_TOKEN} \
                --name mydev-laptop \
                --labels mydev,build,lint,sca,test \
                --unattended \
                --replace
CMD ./run.sh
