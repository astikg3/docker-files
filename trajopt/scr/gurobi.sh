#!/bin/bash

expect_sh=$(expect -c "
	spawn grbgetkey c8e62080-0fc1-7665-e7ca-539b91f53fcf
	expect \"Assign license key to user 'root' on this machine? \[Y/n\]\"
	send \"Y\"
	")

echo "$expect_sh"

