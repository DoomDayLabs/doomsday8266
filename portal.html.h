R"(
<html>
    <head>
        <style>
            *{
                font-family: 'Arial';
            }
            h1{
                text-align: center;
            }
            label{
                display:block;                
            }
            
            form>div{
                margin-bottom: 10px;
            }
            
            input[type='text'],
            input[type='password']{
                width: 100%;
                height: 40px;
                font-size: 20px;
                border-radius: 4px;
                border:1px solid black;
                padding: 10px;
            }
            
            .btn{
                display: inline-block;                
                border: 1px solid black;
                padding: 10px 20px;
                border-radius: 4px;  
                cursor: pointer;
            }
            
            .btn-success{
                background-color: #33bc4d;
                border-color: #0b9826;
                color:white;                    
            }
            
            .btn-warning{
                background-color: #ead63b;
                border-color: #a89921;
                color:white;                    
            }
            
            .pull-right{
                float: right;
            }          
            
            #loader{
                background-image:  url('data:image/gif;base64,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');
                width: 94px;
                height: 92px;
                position: absolute;
                left:50%;
                margin-left: -47px;           
                animation: loading 1s;
                animation-timing-function: steps(11,start);
                animation-iteration-count: infinite;
            }
            
            @keyframes loading{
                from {
                    transform: rotate(0deg);
                }
                to {
                    transform: rotate(330deg);
                }
            }
            
            #form{
                display: none;
            }
            
        </style>
        <script>
            
        function setLoad(visible){
            if (visible===true){
                document.getElementById('form').style['display'] = 'block';
                document.getElementById('loader').style['display'] = 'none';                
            }else{
                document.getElementById('form').style['display'] = 'none';
                document.getElementById('loader').style['display'] = 'block';                
            }
        }
                    
        function onLoad(){
            setTimeout(loadData,1000);
            //loadData();
        }
            
        function saveData(){
            setLoad(false);
            setTimeout(save,1000);
        }
        function save(){
            setLoad(false);
            r = {
                'ssid': document.getElementById('ssid').value,
                'pass': document.getElementById('pass').value,
                'pincode': document.getElementById('pincode').value
            };
            
            var x = new XMLHttpRequest();
            x.onreadystatechange = function() {
              if (x.readyState == 4) {
                 if(x.status == 200) {       
                    setLoad(true);
                }
              }
            };            
            x.open('GET','/save?cfg='+JSON.stringify(r),false);
            x.send(null);
            
        }
            
            
        function reboot(){
            
        }
            
            
        
            
        function loadData(){
            var x = new XMLHttpRequest();
            x.onreadystatechange = function() {
              if (x.readyState == 4) {
                 if(x.status == 200) {                    
                    r = JSON.parse(x.responseText);                    
                    document.getElementById('ssid').value = r['ssid'];
                    document.getElementById('pass').value = r['pass'];
                    document.getElementById('pincode').value = r['pincode'];
                    setLoad(true);
                }
              }
            };
            x.open('GET','/load',false);
            x.send(null);                        
        }
                    
        </script>
        <meta name='viewport' content='width=device-width, initial-scale=1' />
    </head>
    <body onload='onLoad()'>
        <h1>Doom Day Device</h1>
        <div id='loader'></div>
        <form id='form'>
            <div>
                <label> WiFi SSID </label>
                <input type='text' id='ssid' value='TEXT'/>
            </div>
            <div>
                <label> WiFi Key </label>
                <input type='password' id='pass' value='TEXT'/>
            </div>
            <div>
                <label> Devie PINCODE </label>
                <input type='text' id='pincode' value='TEXT'/>
            </div>
            <div class='pull-right'>
                <a class='btn btn-success' id='save' onclick='saveData()'>Save</a>
                <a class='btn btn-warning' id='reboot'>Reboot</a>
            </div>
        </form>
    </body>
</html>
)"
