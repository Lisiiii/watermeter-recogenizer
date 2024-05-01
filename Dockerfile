FROM ubuntu:22.04

SHELL ["/bin/bash", "-c"]

# install basic tools

RUN apt-get update && apt-get -y install \
    vim wget curl \
    gcc g++ cmake \
    python3 python3-pip 

# install oh my zsh & change theme to af-magic
RUN apt-get update && apt-get -y install zsh git
RUN wget https://gitee.com/mirrors/oh-my-zsh/raw/master/tools/install.sh -O zsh-install.sh && \
    chmod +x ./zsh-install.sh && ./zsh-install.sh && \
    sed -i 's/REPO=${REPO:-ohmyzsh\/ohmyzsh}/REPO=${REPO:-mirrors\/oh-my-zsh}/' ./zsh-install.sh && \
    sed -i 's/REMOTE=${REMOTE:-https:\/\/github.com\/${REPO}.git}/REMOTE=${REMOTE:-https:\/\/gitee.com\/${REPO}.git}/' ./zsh-install.sh && \
    sed -i 's/ZSH_THEME=\"[a-z0-9\-]*\"/ZSH_THEME="af-magic"/g' ~/.zshrc && \
    sed -i 's/plugins=(git)/plugins=(git zsh-syntax-highlighting zsh-autosuggestions)/g' ~/.zshrc  && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting && git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions && \
    rm ./zsh-install.sh 

RUN chsh root -s /bin/zsh