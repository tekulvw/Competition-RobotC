#ifndef LG3_C
#define LG3_C

#define L3G4200D_ADDRESS_SA0_LOW  (0xD0 >> 1)
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2 >> 1)
#define L3GD20_ADDRESS_SA0_LOW    (0xD4 >> 1)
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6 >> 1)

bool LG3_init(LG3* sensor, byte device, byte sa0) {
	sensor->_device = device;
	switch (sensor->_device) {
		case L3G4200D_DEVICE:
			if (sa0 == L3G_SA0_LOW) {
				sensor->address = L3G4200D_ADDRESS_SA0_LOW;
				return true;
			} else if (sa0 == L3G_SA0_HIGH) {
				sensor->address = L3G4200D_ADDRESS_SA0_HIGH;
				return true;
			} else {
				return autoDetectAddress(sensor);
			}
			break;
		case L3GD20_DEVICE:
			if (sa0 == L3G_SA0_LOW) {
				sensor->address = L3GD20_ADDRESS_SA0_LOW;
			} else if (sa0 == L3G_SA0_HIGH) {
				sensor->address = L3GD20_ADDRESS_SA0_HIGH;
			} else
				return autoDetectAddress(sensor);
			break;
		default:
			return autoDetectAddress(sensor);
	}
}

void enableDefault(const LG3* this) {
	//0x0F = 0b00001111
	//Normal power mode, all axes enabled
	writeReg(this, L3G_CTRL_REG1, 0x0F);
}

void writeReg(const LG3* this, byte reg, byte value) {
	i2c_beginTransmission(&(this->port),this->address);
	i2c_write(&(this->port),reg);
	i2c_write(&(this->port),value);
	i2c_endTransmission(&(this->port));
}

byte readReg(const LG3* this, byte reg)
{
  byte value;

  i2c_beginTransmission(&(this->port),this->address);
  i2c_write(&(this->port),reg);
  i2c_endTransmission(&(this->port));
  i2c_requestFrom(&(this->port), this->address, (byte)1);
  value = i2c_read(&(this->port));
  i2c_endTransmission(&(this->port));

  return value;
}

void read(LG3* this) {
  i2c_beginTransmission(&(this->port),this->address);
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  i2c_write(&(this->port),L3G_OUT_X_L | (1 << 7));
  i2c_endTransmission(&(this->port));
  i2c_requestFrom(&(this->port),this->address, (byte)6);

  while (i2c_available(&(this->port)) < 6) {}

  short xlg = i2c_read(&(this->port));
  short xhg = i2c_read(&(this->port));
  short ylg = i2c_read(&(this->port));
  short yhg = i2c_read(&(this->port));
  short zlg = i2c_read(&(this->port));
  short zhg = i2c_read(&(this->port));

  // combine high and low bytes
  this->g.x = (int)(xhg << 8 | xlg);
  this->g.y = (int)(yhg << 8 | ylg);
  this->g.z = (int)(zhg << 8 | zlg);
}

void vector_cross(const vector *a,const vector *b, vector *out)
{
  out->x = a->y*b->z - a->z*b->y;
  out->y = a->z*b->x - a->x*b->z;
  out->z = a->x*b->y - a->y*b->x;
}

float vector_dot(const vector *a,const vector *b)
{
  return a->x*b->x+a->y*b->y+a->z*b->z;
}

void vector_normalize(vector *a)
{
  float mag = sqrt(vector_dot(a,a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}

bool autoDetectAddress(LG3* this) {
	// try each possible address and stop if reading WHO_AM_I returns the expected response
	this->address = L3G4200D_ADDRESS_SA0_LOW;
	if (readReg(this, L3G_WHO_AM_I) == 0xD3) return true;
	this->address = L3G4200D_ADDRESS_SA0_HIGH;
	if (readReg(this, L3G_WHO_AM_I) == 0xD3) return true;
	this->address = L3GD20_ADDRESS_SA0_LOW;
	if (readReg(this, L3G_WHO_AM_I) == 0xD4 || readReg(this, L3G_WHO_AM_I) == 0xD7) return true;
	this->address = L3GD20_ADDRESS_SA0_HIGH;
	if (readReg(this, L3G_WHO_AM_I) == 0xD4 || readReg(this, L3G_WHO_AM_I) == 0xD7) return true;

	return false;
}


#endif
