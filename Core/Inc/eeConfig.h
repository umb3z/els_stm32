#ifndef __EECONFIG_H
#define __EECONFIG_H

#define   _EE_USE_FLASH_PAGE_OR_SECTOR              (126) //stm32f103c8t6 ha 127 pagine, utilizzerò l'ultima disponibile
#define   _EE_USE_RAM_BYTE                          (1024) //stm32f103c8t6 ogni pagina è di 1024byte
#define   _EE_VOLTAGE                               FLASH_VOLTAGE_RANGE_3 //  use in some devices
#endif
