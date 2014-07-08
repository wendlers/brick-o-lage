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

Blockly.Python['bol_dio_input_event'] = function(block) {
  var dropdown_brick = block.getFieldValue('brick');
  var dropdown_port = block.getFieldValue('port');
  var value_match = Blockly.Python.valueToCode(block, 'match', Blockly.Python.ORDER_ATOMIC);
  var statements_block = Blockly.Python.statementToCode(block, 'block');

  var func_cb   = 'cb_' + dropdown_brick + '_' + dropdown_port + '_' + value_match;
  var func_exec = 'exec_' + dropdown_brick + '_' + dropdown_port + '_' + value_match;

  var code = 
  	  'def ' + func_exec + '():\n' + statements_block + '\n\n'
    + 'def ' + func_cb + '(value):\n'
  	+ '  if value == ' + value_match + ':\n'
  	+ '    ' + func_exec + '()\n\n'
  	+ 'bol.addCallback("' + dropdown_brick + '", "' + dropdown_port + '", ' + func_cb + ')\n';		  

  return code;
};

Blockly.Python['bol_sleep'] = function(block) {
  var dropdown_unit = block.getFieldValue('unit');
  var value_amount = Blockly.Python.valueToCode(block, 'amount', Blockly.Python.ORDER_ATOMIC);

  var code = '';

  if(dropdown_unit == 'sec')
  {
    code = 'bol.sleep(' + value_amount + ')\n';
  }
  else if(dropdown_unit == 'msec')
  {
    code = 'bol.msleep(' + value_amount + ')\n';
  }
  
  return code;
};

Blockly.Python['bol_repeat_forever'] = function(block) {
  var statements_block = Blockly.Python.statementToCode(block, 'block');
  
  var code = 'while True:\n  bol.usleep(1000)\n' + statements_block;

  return code;
};

Blockly.Python['bol_dcm_direction'] = function(block) {
  var dropdown_brick = block.getFieldValue('brick');
  var dropdown_port = block.getFieldValue('port');
  var dropdown_direction = block.getFieldValue('direction');
  
  var code = 'bol.set_port("' + dropdown_brick + '", "' + dropdown_port + '", ' + dropdown_direction + ')\n';
 
  return code;
};

Blockly.Python['bol_dcm_speed'] = function(block) {
  var dropdown_brick = block.getFieldValue('brick');
  var dropdown_port = block.getFieldValue('port');
  var value_speed = Blockly.Python.valueToCode(block, 'speed', Blockly.Python.ORDER_ATOMIC);
 
  var code = 
  	'bol.set_port("' + dropdown_brick + '", "' + dropdown_port + '_CW", '  + value_speed + ')\n' +
    'bol.set_port("' + dropdown_brick + '", "' + dropdown_port + '_CCW", ' + value_speed + ')\n';
 
  return code;
};

Blockly.Python['bol_http_post_req'] = function(block) {
  var value_req_url = Blockly.Python.valueToCode(block, 'req_url', Blockly.Python.ORDER_ATOMIC);
  var value_req_fields = Blockly.Python.valueToCode(block, 'req_fields', Blockly.Python.ORDER_ATOMIC);
  var text_fields = block.getFieldValue('fields');
  var text_url = block.getFieldValue('url');
 
  var code = 'bol.http_post_req("' + text_url + '", "' + encodeURIComponent(text_fields) + '")\n';
 
  return code;
};

Blockly.Python['bol_http_get_req'] = function(block) {
  var value_req_url = Blockly.Python.valueToCode(block, 'req_url', Blockly.Python.ORDER_ATOMIC);
  var value_req_fields = Blockly.Python.valueToCode(block, 'req_fields', Blockly.Python.ORDER_ATOMIC);
  var text_fields = block.getFieldValue('fields');
  var text_url = block.getFieldValue('url');
 
  var code = 'bol.http_get_req("' + text_url + '", "' + encodeURIComponent(text_fields) + '")\n';
 
  return code;
};