//types définis pour le projet terraZoo


//définition de types
typedef enum bool_e
{
  false=0,
  true=1
}bool_e;

typedef struct terraZooData_s
{
  int16_t theTemp;
  int16_t theLight;
  bool_e isLightOn;
  bool_e isHeaterOn;
  int16_t theTempConsigne;
  int16_t theLightConsigne;
}terraZooData_s;