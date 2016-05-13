     ____               _        _       ___      _____ 
    |  _ \  _ __  ___  (_)  ___ | |_    |_ _| ___|_   _|
    | |_) || '__|/ _ \ | | / _ \| __|    | | / _ \ | |  
    |  __/ | |  | (_) || ||  __/| |_     | || (_) || |  
    |_|    |_|   \___/_/ | \___| \__|   |___|\___/ |_|  
                     |__/                              
            
# Contributeurs
- Cyril
- Guillaume
- Mattias
- Léo

# Structure des données en JSON
     {
          cpu: {
               user,
               nice,
               system,
               idle,
               pcentUsed
          },
          ram: {
               total,
               free,
               buffers,
               cached,
               used,
               pcentUsed
          },
          swap: {
               total,
               free,
               cached,
               used,
               pcentUsed
          },
          disk: {
               totalRead,
               totalWrite,
               totalTimeActive,
               debitRead,
               debitWrite,
               pcentActive
          },
          network: {
               totalDown,
               totalUp,
               debitDown,
               debitUp
          },
          time: {
               sec,
               microsec
          }
     }
