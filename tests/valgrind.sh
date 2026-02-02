#!/bin/bash

# Build and run Valgrind in Docker

echo "Building Docker image..."
docker build -t minishell-valgrind ..

echo ""
echo "Running Valgrind..."
echo "======================================"
docker run -it --rm minishell-valgrind

