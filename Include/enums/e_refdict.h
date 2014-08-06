#ifndef E_REFDICT_H
#define E_REFDICT_H

/* Słownik dbRef
 * przydatny przy tworzeniu obiektów
 * przez metody fabryczne, które
 * wymagają podania ref obiektu do stworzenia
 */
namespace refDict
{
  enum class Item
  {
    Miedziak = 121
  };
  enum class Location
  {
    StartLocation = 1
  };
}

#endif // E_REFDICT_H
