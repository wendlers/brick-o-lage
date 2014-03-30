Blockly.Python['brick_output'] = function(block) {
  var value_brick_name = Blockly.Python.valueToCode(block, 'brick_name', Blockly.Python.ORDER_ATOMIC);
  var value_brick_port = Blockly.Python.valueToCode(block, 'brick_port', Blockly.Python.ORDER_ATOMIC);
  var value_brick_port_value = Blockly.Python.valueToCode(block, 'brick_port_value', Blockly.Python.ORDER_ATOMIC);
  var code = 'bol.set_port(' + value_brick_name + ', ' + value_brick_port + ', ' + value_brick_port_value + ')\n';
  return code;
};

Blockly.Python['sleep_sec'] = function(block) {
  var value_seconds = Blockly.Python.valueToCode(block, 'seconds', Blockly.Python.ORDER_ATOMIC);
  var code = 'bol.sleep(' + value_seconds + ')\n';
  return code;
};
