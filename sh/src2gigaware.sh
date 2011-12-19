#! /bin/bash
rsync -rtvO --delete --modify-window=1 ~/src /media/gigaware/.
rsync -rtvO --delete --modify-window=1 ~/other /media/gigaware/.
rsync -rtvO --delete --modify-window=1 ~/workspace /media/gigaware/.

