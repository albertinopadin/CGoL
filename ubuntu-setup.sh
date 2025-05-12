# Install dependencies:
sudo apt install glew-utils libglew2.2 libglfw3 libomp-dev

# Copy files if not present:
sudo cp -u dependencies/include/GL/glew.h /usr/include/GL/
sudo mkdir -p /usr/include/GLFW
sudo cp -u dependencies/include/GLFW/glfw3.h /usr/include/GLFW/
sudo cp -u dependencies/include/GLFW/glfw3native.h /usr/include/GLFW/

# Make build directory:
sudo mkdir -p build