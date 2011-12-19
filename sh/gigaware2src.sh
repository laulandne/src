#! /bin/bash
rsync -rtvO --delete --modify-window=1 /media/gigaware/src ~/.
rsync -rtvO --delete --modify-window=1 /media/gigaware/other ~/.
rsync -rtvO --delete --modify-window=1 /media/gigaware/workspace ~/.
