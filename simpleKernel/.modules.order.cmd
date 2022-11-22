cmd_/home/jipi/simpleKernel/modules.order := {   echo /home/jipi/simpleKernel/hello.ko; :; } | awk '!x[$$0]++' - > /home/jipi/simpleKernel/modules.order
