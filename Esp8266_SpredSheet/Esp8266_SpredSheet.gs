var ss = SpreadsheetApp.openById('1YjlEimTa6MOZJPlojDc7fKnFbjzVC11Wsaj_IcNYx7g');
var sheet = ss.getSheetByName('Data');
var menu = ss.getSheetByName('Menu');

function doGet(e){
  var tag = e.parameters.tag;
  var val = e.parameter.value;
  var read = e.parameter.read;
  
   
  if (read !== undefined){
    return ContentService.createTextOutput(menu.getRange('C2').getValue());
  }
  //-------------------------------------------------------------------
  var row = sheet.getLastRow() + 1;
    
  var now = Utilities.formatDate(new Date(), "EST", "dd/mm/yy'*****'hh:mm a");
    
    
  
  var range = sheet.getRange("C" + row);    var retval = range.setValue(val).getValue();
  sheet.getRange("A" + row).setValue(row -1).getValue();
  sheet.getRange("B" + row).setValue(now).getValue();
  sheet.getRange("D" + row).setValue(tag).getValue();
  
  
  
  
  
  
  
  
  
  
  //----------------------------------------------------------------
  if (retval == e.parameter.value)
    return ContentService.createTextOutput("Successfully wrote: " + e.parameter.value + "\ninto spreadsheet.");
  else
    return ContentService.createTextOutput("Unable to write into spreadsheet.\nCheck authentication and make sure the cursor is not on cell 'A2'." + retval + ' ' + e.parameter.value);
}



