#!/bin/sh
# El propósito de este script es configurar git
# para que utilice los hooks que se encuentran
# en el directorio .git-hooks

git config core.hooksPath $(pwd)/.git-hooks
