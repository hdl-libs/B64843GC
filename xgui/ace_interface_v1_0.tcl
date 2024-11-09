# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"

}

proc update_PARAM_VALUE.EXPAND_ADDRESS_BITS { PARAM_VALUE.EXPAND_ADDRESS_BITS } {
	# Procedure called to update EXPAND_ADDRESS_BITS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.EXPAND_ADDRESS_BITS { PARAM_VALUE.EXPAND_ADDRESS_BITS } {
	# Procedure called to validate EXPAND_ADDRESS_BITS
	return true
}

proc update_PARAM_VALUE.VALID_ADDRESS_BITS { PARAM_VALUE.VALID_ADDRESS_BITS } {
	# Procedure called to update VALID_ADDRESS_BITS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.VALID_ADDRESS_BITS { PARAM_VALUE.VALID_ADDRESS_BITS } {
	# Procedure called to validate VALID_ADDRESS_BITS
	return true
}


proc update_MODELPARAM_VALUE.EXPAND_ADDRESS_BITS { MODELPARAM_VALUE.EXPAND_ADDRESS_BITS PARAM_VALUE.EXPAND_ADDRESS_BITS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.EXPAND_ADDRESS_BITS}] ${MODELPARAM_VALUE.EXPAND_ADDRESS_BITS}
}

proc update_MODELPARAM_VALUE.VALID_ADDRESS_BITS { MODELPARAM_VALUE.VALID_ADDRESS_BITS PARAM_VALUE.VALID_ADDRESS_BITS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.VALID_ADDRESS_BITS}] ${MODELPARAM_VALUE.VALID_ADDRESS_BITS}
}

