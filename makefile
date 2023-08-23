runtime:
	gcc -o ttybrowser main.c net.c

debug:
	gcc -g -o ttybrowser_debug main.c net.c
	lldb ttybrowser_debug
