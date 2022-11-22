cmd_/home/jipi/director/modules.order := {   echo /home/jipi/director/sumofnumbers.ko; :; } | awk '!x[$$0]++' - > /home/jipi/director/modules.order
