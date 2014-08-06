#ifndef GO_H
#define GO_H

#include <map>
#include "Include/enums/e_direction.h"
#include "command.h"

class Location;

class Go : public NonActiveCommand
{
public:  
  Go();
  virtual ~Go() {}

  virtual CommandID id() const;
  virtual void execute(std::vector<std::string> params);
  virtual void execute(std::string = "");

private:
  void rozejrzyjSie();
  void movePlayer(Direction);
  void movePlayerSuccess();
  void movePlayerFail();

  void displayLocationInfo(Location* loc);
  void displayDivider();
  void displayLocName(Location* loc);
  void displayPossibleExits(Location* loc);
  void displayLocDescription(Location* loc);
  void dispalyLocationObjects(Location *loc);
  void displayCreaturesInLoc(Location *loc);

  void loadNeighbourLocations(Location* loc);
  std::map<Direction, bool> getPossibleExits(Location *loc);
};

#endif // GO_H
