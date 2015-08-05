install:
	cp robot.sh /etc/init.d/
	chmod +x /etc/init.d/robot.sh
	update-rc.d robot.sh defaults

uninstall:
	update-rc.d robot.sh remove
	rm /etc/init.d/robot.sh
