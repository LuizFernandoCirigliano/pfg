#include "shape.hpp"
#include "helpers.hpp"

Shape::Shape(int clientID, const char* name, simxFloat (&position)[3],
  simxFloat (&orientation)[3]) :
    VRepClass(clientID, name)
{
  for (int i = 0; i < 3; i++) {
    _initialOrientation[i] = orientation[i];
    _initialPosition[i] = position[i];
  }
  this->reset();
}

Shape::Shape(int clientID, simxInt handle) :
  VRepClass(clientID, handle)
{
  _name = "";
  simxGetObjectPosition(clientID, handle, sim_handle_parent, _initialPosition, simx_opmode_blocking);
  simxGetObjectOrientation(clientID, handle, sim_handle_parent, _initialOrientation, simx_opmode_blocking);
}

void Shape::reset() {
  int ret = simxSetObjectPosition(_clientID, _handle, sim_handle_parent,
    _initialPosition, simx_opmode_oneshot);

  if (ret) {
    std::cout << "**ERROR: setting position for shape " << _handle << std::endl;
    log_response(ret);
  }
  ret = simxSetObjectOrientation(_clientID, _handle, sim_handle_parent,
    _initialOrientation, simx_opmode_oneshot);

  if (ret) {
    std::cout << "**ERROR: setting orientation for shape " << _handle << std::endl;
    log_response(ret);
  }
}

std::ostream& operator<< (std::ostream& stream, const Shape& shape) {
  stream << "Shape name: " << shape._name << " Handle: " << shape._handle << std::endl;
  stream << "Initial position: " << "[" << shape._initialPosition[0] << ", " <<
    shape._initialPosition[1] << ", " << shape._initialPosition[2] << "]" << std::endl;
  return stream;
}
