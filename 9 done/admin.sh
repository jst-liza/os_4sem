# !/bin/bash
echo "All cap_values removed";
setcap -r 9_test;
echo "Set up necessary capability?Y|N"
read var
if  [ "$var" = "Y" ] || [ "$var" = "y" ]
then
	echo "New cap_value set:";
	setcap CAP_DAC_OVERRIDE=+eip 9_test;
	getcap 9_test;
else
	echo "Raw values remained";
fi
xattr -l 9_test;
