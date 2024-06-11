# Use a minimal base image
FROM ubuntu:latest

# Set a working directory
WORKDIR /app

# Copy the precompiled binary into the container
COPY cmake-build-release/bin/holdr /app/

# Copy the main configuration file into the container
COPY config.toml /app/

# Copy the dns zone file into examples folder
COPY examples/example-com.db /app/examples/

# Grant execution rights to the binary
RUN chmod +x /app/holdr

# Expose the necessary port
EXPOSE 5354

# Specify the binary to be run when the container starts
CMD ["./holdr", "./config.toml"]

# Bellow are basic commands used to build and run holdr in docker
# This file is only exemplary altough it works you should customize it to your needs
# docker build -t holdr .
# docker run -p 5354:5354/udp --rm holdr