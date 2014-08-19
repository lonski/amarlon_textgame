#ifndef DATA_GATEWAY_H
#define DATA_GATEWAY_H

class DBObject;

class DataGateway
{
public:
  DataGateway() {}
  virtual ~DataGateway() {}

  virtual DBObject* fetch(int id) = 0;
  virtual void write(DBObject* obj) = 0;
};

#endif // DATA_GATEWAY_H
