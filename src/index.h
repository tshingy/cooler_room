const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html><html><head><meta content='width=device-width, initial-scale=1' name='viewport'/><title>Cooler Room Status</title>
  <style>
  .divTable{
  display: table;
  width: 100%;
}
.divTableRow {
  display: table-row;
  font-size: 1.5em;
}
.divTableHeading {
  background-color: #EEE;
  display: table-header-group;
}
.divTableCell, .divTableHead {
  display: table-cell;
  padding: 3px 10px;
}
.divTableHeading {
  background-color: #EEE;
  display: table-header-group;
  font-weight: bold;
}
.divTableFoot {
  background-color: #EEE;
  display: table-footer-group;
  font-weight: bold;
}
.divTableBody {
  display: table-row-group;
}
</style>
   </head><body bgColor="black" style="margin:auto; padding: 10px; text-align: center; color: gray;">
   
  <div id="updating" style="margin-top:50px;">
  <div style="margin-bottom: 20px; text-align: center; font-size: 2.5em; text-decoration: underline;">HEATER OFF</div>
  <div class="divTable" style="width: auto; margin: auto;">
    <div class="divTableBody">
      <div class="divTableRow">
        <div class="divTableCell" style="text-align: right;">Room Temp:</div>
        <div class="divTableCell" style="text-align: left;">0</div>
      </div>
      <div class="divTableRow">
        <div class="divTableCell" style="text-align: right;">AC Temp:</div>
        <div class="divTableCell">0</div>
      </div>
      <div class="divTableRow">
        <div class="divTableCell" style="text-align: right;">&nbsp;</div>
        <div class="divTableCell">&nbsp;</div>
      </div>
      <div class="divTableRow">
        <div class="divTableCell" style="text-align: right;">Room Setting:</div>
        <div class="divTableCell">0</div>
      </div>
      <div class="divTableRow">
        <div class="divTableCell" style="text-align: right;">AC Setting:</div>
        <div class="divTableCell">0</div>
      </div>
    </div>
  </div>
  </div>
  
  <br />
    <form action="set_value" method="post">
    <div class="divTable" style="width: auto; margin: auto;">
      <div class="divTableBody">
        <div class="divTableRow">
          <div class="divTableCell" style="text-align: right;">Set Room Temp</div>
          <div class="divTableCell" style="text-align: left;"><input type="number" name="set_room" id="set_room" style="width:50px;" /></div>
        </div>
        <div class="divTableRow">
          <div class="divTableCell" style="text-align: right;">Min AC Temp</div>
          <div class="divTableCell" style="text-align: left;"><input type="number" name="set_ac" id="set_ac" style="width:50px;" /></div>
        </div>
        <div class="divTableRow">
          <div class="divTableCell" style="text-align: right;"><input type="submit" value="Save" style="font-size: 0.75em; /></div>
          <div class="divTableCell">&nbsp;</div>
        </div>
      </div>
    </div>
  </form>
  
    
    <script>
    var x = setInterval(function() {loadData("updating",updateData)}, 1000);
    function loadData(url, callback){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function(){
     if(this.readyState == 4 && this.status == 200){
     callback.apply(xhttp);
     }
    };
    xhttp.open("GET", url, true);
    xhttp.send();
    }
    function updateData(){
     document.getElementById("updating").innerHTML = this.responseText;
    }    
    </script>

    </body></html>
)=====";
