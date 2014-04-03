Blockly.Python['bol_dio_output'] = function(block) {
  var dropdown_brick = block.getFieldValue('brick');
  var dropdown_port = block.getFieldValue('port');
  var value_out = Blockly.Python.valueToCode(block, 'out', Blockly.Python.ORDER_ATOMIC);

  var code = 'bol.set_port("' + dropdown_brick + '", "' + dropdown_port + '", ' + value_out + ')\n';

  return code;
};

Blockly.Python['bol_dio_input'] = function(block) {
  var dropdown_brick = block.getFieldValue('brick');
  var dropdown_port = block.getFieldValue('port');

  var code = 'bol.get_port("' + dropdown_brick + '", "' + dropdown_port + '")';
  
  return [code, Blockly.Python.ORDER_NONE];
};

Blockly.Python['bol_sleep'] = function(block) {
  var dropdown_unit = block.getFieldValue('unit');
  var value_amount = Blockly.Python.valueToCode(block, 'amount', Blockly.Python.ORDER_ATOMIC);

  var code = 'bol.sleep(' + value_amount + ')\n';

  return code;
};