Blockly.Blocks['bol_dio_output'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(290);
    this.appendDummyInput()
        .appendField("set DIO")
        .appendField(new Blockly.FieldDropdown([["1", "DIO1"], ["2", "DIO2"], ["3", "DIO3"], ["4", "DIO4"]]), "brick");
    this.appendDummyInput()
        .appendField("DO")
        .appendField(new Blockly.FieldDropdown([["1", "DO1"], ["2", "DO2"], ["3", "DO3"], ["4", "DO4"]]), "port");
    this.appendValueInput("out")
        .setCheck("Number")
        .appendField("value");
    this.setInputsInline(true);
    this.setPreviousStatement(true, "null");
    this.setNextStatement(true, "null");
    this.setTooltip('');
  }
};

Blockly.Blocks['bol_dio_input'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(330);
    this.appendDummyInput()
        .appendField("get DIO")
        .appendField(new Blockly.FieldDropdown([["1", "DIO1"], ["2", "DIO2"], ["3", "DIO3"], ["4", "DIO4"]]), "brick");
    this.appendDummyInput()
        .appendField("DI")
        .appendField(new Blockly.FieldDropdown([["1", "DI1"], ["2", "DI2"], ["3", "DI3"], ["4", "DI4"]]), "port");
    this.setInputsInline(true);
    this.setOutput(true, "Number");
    this.setTooltip('');
  }
};

Blockly.Blocks['bol_dio_input_event'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(20);
    this.appendDummyInput()
        .appendField("whenever DIO")
        .appendField(new Blockly.FieldDropdown([["1", "DIO1"], ["2", "DIO2"], ["3", "DIO3"], ["4", "DIO4"]]), "brick");
    this.appendDummyInput()
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("DI")
        .appendField(new Blockly.FieldDropdown([["1", "DI1"], ["2", "DI2"], ["3", "DI3"], ["4", "DI4"]]), "port");
    this.appendDummyInput()
        .appendField("value");
    this.appendValueInput("match")
        .setCheck("Number")
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField("");
    this.appendStatementInput("block")
        .setCheck("null");
    this.setInputsInline(true);
    this.setTooltip('');
  }
};

Blockly.Blocks['bol_sleep'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(65);
    this.appendDummyInput()
        .appendField("sleep")
        .appendField(new Blockly.FieldDropdown([["sec", "sec"], ["msec", "msec"]]), "unit");
    this.appendValueInput("amount")
        .setCheck("Number")
        .appendField("value");
    this.setInputsInline(true);
    this.setPreviousStatement(true, "null");
    this.setNextStatement(true, "null");
    this.setTooltip('');
  }
};

Blockly.Blocks['bol_repeat_forever'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(20);
    this.appendDummyInput()
        .appendField("repeat forever");
    this.appendStatementInput("block")
        .setCheck("null");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setTooltip('');
  }
};