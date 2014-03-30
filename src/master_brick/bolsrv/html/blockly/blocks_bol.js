Blockly.Blocks['brick_output'] = {
  init: function() {
    this.setHelpUrl('http://gpio.kaltpost.de/');
    this.setColour(285);
    this.appendValueInput("brick_name")
        .setCheck("String")
        .appendField("Brick");
    this.appendValueInput("brick_port")
        .setCheck("String")
        .appendField("Port");
    this.appendValueInput("brick_port_value")
        .setCheck("Number")
        .appendField("Value");
    this.setInputsInline(true);
	this.setPreviousStatement(true);
	this.setNextStatement(true);
    this.setTooltip('Brick-o-Lage output');
  }
};

Blockly.Blocks['sleep_sec'] = {
  init: function() {
    this.setHelpUrl('http://www.example.com/');
    this.setColour(285);
    this.appendValueInput("seconds")
        .setCheck("Number")
        .appendField("Sleep seconds");
    this.setInputsInline(true);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
    this.setTooltip('');
  }
};

