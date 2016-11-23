<html lang="en">
     <head>
     <meta charset="utf-8">
     <title>catchup</title>
     <head>
   <script src="./chart.js/Chart.js"></script>
   <script src="./chart.js/src/Chart.LineBar.js"></script>
   <script src="http://code.jquery.com/jquery-latest.min.js"></script>
   
   <link rel="stylesheet" href="./style.css">

     <style>

         body{
                background-image:url('./image/main.png');
                background-repeat: no-repeat;
                background-size: 360px 615px;
                background-position: top;

              }

            #select{
              border: 1px solid #4CB9C0 ;
              border-opacity: 0.7;
              width : 330px;
              height: 100px;
              //*padding : 3px;*//
              float : center;
              overflow: hidden;
             }


            #home{
                border-right: 1px solid #4CB9C0 ;
                border-opacity: 0.7;
                width: 60px;
                height :80px;
                padding : 10px;
                float : left;
                ; 
                 }

           #graph1{
              border-right: 1px solid #4CB9C0 ;
              border-opacity: 0.7;
              width: 60px;
              height :80px;
              padding : 10px;
              float : left;
               }

           #health1{
              border-right: 1px solid #4CB9C0 ;
              border-opacity: 0.7;
              width: 60px;
              height :80px;
              padding : 10px;
              float : left;
               }


           #setting{
              width: 60px;
              height :80px;
              padding : 10px;
              float : left;
               }


           .line{
              /*margin:50px auto;*/ 
              width : 400px;
              height: 230px;
              float : center;
              margin-top: 20px;
              margin-right: px;
              border: 2px double;
              border-color:rgba(76,185,192,0.5);
            
           }


           canvas{
            margin-top: 13px;
           }

          

            .line2{
                 /* margin:50px auto;*/
                  width:400px;
                  height: 200px;
                  float:center;
                  margin-bottom: 10px;
                  border: 2px double;
                  border-color:rgba(76,185,192,0.5);
          }

                  #text {
         color : #EEF2F3;
         font-weight: bold;
         font-size : 2em;
         font-family: "Comic Sans MS";
         font-opacity : 0.7;
          }

        #head{
         color : #EEF2F3;
         font-weight: bold;
         font-size : 15pt;
         font-family: "Comic Sans MS"
          }

        #middle{
          color : #EEF2F3;
          opacity : 0.7;
          font-family: "휴먼엽서체";
          }


	#g1 {
        width:500px; height:220px;
        display: inline-block;
        margin-left:-40px;
	margin-top:-130px;
        
      }
      img {
      	width:180px;
      	height:auto;
      	margin-left:-50px;
      	margin-top:-140px;
      	
      }
      #test {
      	width:100%;
      	height:130px;
      }
      #test1{
      	width:500px;
      	height:10px;
      }
	#png {
		margin-top:50px;
		margin-left:40px;
	}
     #p1 {
		margin-top:70px;
}


     </style>

       </head>

      

    <body>

     <center>
      <script src="./justgage-master/justgage-master/raphael-2.1.4.min.js"></script>
    <!--<script src="../justgage-1.0.1.js"></script>-->
    <script src="./justgage-master/justgage-master/justgage.js"></script>
    <script>
    /*document.addEventListener("DOMContentLoaded", function(event) {

      var g1 = new JustGage({
        id: "g1",
        value: 100,
        min: 0,
        max: 100,
        label: "",
        gaugeWidthScale: 0.08,
        levelColors : [  "#4AB9C0", "#4AB9C0",  "#4AB9C0" ],
        hideValue:true

      });  
         //g1.refresh();
    });*/
    </script>

	<div id ="main" style="
         /*padding-top:50px; */
         margin-top: 30px;
         border : 5px double #4CB9C0;
         height: 400px;
         width: 330px;
        ">

         
<p id="p1" style="color:white; font-size:50px"></p>
<div id="png">
<div id="test1'">
	<div id="test"><div id="g1" style="width:330px; height:290px"></div></div>
       <img id='img' src='' width="250px" height="300px"></div>
</div>
</div>


       <br>
         &nbsp; 

          <div id ="select">
            
            <div id="home"
            style ="background:url('./image/home.png');
            background-repeat:no-repeat;
            background-position: center;
            background-size: 60px,55px;
            cursor:pointer;
            background-margin-top: 7px;
            margin-top: 7px;"
            onclick ="location.href='catchup_alarm.php'">
            </div>

             <div id="graph1"
            style ="background:url('./image/graph.png');
            background-repeat:no-repeat;
            background-position: center;
            background-size: 70px,75px;
            background-margin-top: 7px;
            margin-top: 7px;
            cursor:pointer;"
            onclick ="location.href='catchup_graph2.php'">
            </div>


            <div id="health1"
            style ="background:url('./image/health.png');
            background-repeat:no-repeat;
            background-position: center;
            background-size: 75px,120px;
            background-margin-top: 10px;
            cursor:pointer;
            margin-top: 10px;"
            onclick ="location.href='catchup_health.html'">
            </div>


            <div id="setting"
            style ="background:url('./image/setting.png');
            background-repeat:no-repeat;
            background-position: center;
            background-size: 70px,70px;
            background-margin-top: 5px;
            background-margin-left: 10px;
            margin-top: 7px;
            cursor:pointer;"
            onclick ="location.href='catchup_setting.html'">
            </div> 
            
         
      </div>

<canvas id="canvas" width="1000" height="1000"></canvas>
<script>
  var ctx = document.getElementById('canvas').getContext('2d');
  var log=function(s){
    console.log(s);
    if(document.readyState != 'complete') {
      log.buffer.push(s);
    }else {
      //document.getElementById('output').innerHTML +=(s+'\n');

    }
  }
  log.buffer=[];
  url='ws://14.63.213.137:8080';
  var w=new WebSocket(url);

   //document.addEventListener("DOMContentLoaded", function(event) {

 var get1= new JustGage({
        id:"g1",
        value:"100",
        min: 0,
        max: 100,
        label: "",
        gaugeWidthScale: 0.08,
        levelColors : [  "#4AB9C0", "#4AB9C0",  "#4AB9C0" ],
        hideValue:true,
	hideMinMax:true
	//console.log(get1.id);
      });
         //g1.refresh();
	
    //});
   

  w.onopen=function(){
    log('open');
    w.send('thank you for accepting this Web socket request');
    //ctx.fillStyle = "#0095DD";
             //ctx.fillRect(150, 30, 100, 100);

  }
  w.onmessage=function(e){
    console.log(e.data);
    log(e.data);
    
    if(e.data<70){
      document.getElementById('img').src="/images/65.png";  
      document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>=70 && e.data<=75){
      document.getElementById('img').src="/images/75.png";
      document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>75 && e.data<=80){
      document.getElementById('img').src="/images/80.png";
      document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>80 && e.data<=85){
      document.getElementById('img').src="/images/85.png";
      document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>85 && e.data<=90){
	document.getElementById('img').src="/images/90.png";
        document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>90 && e.data<=95){
	document.getElementById('img').src="/images/95.png";
        document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>95 && e.data<=100){
	document.getElementById('img').src="/images/100.png";
        document.getElementById('p1').innerHTML=e.data;
    }
    else if(e.data>100 && e.data<=105){
	document.getElementById('img').src="/images/105.png";
        document.getElementById('p1').innerHTML=e.data;
    }
    else{
      document.getElementById('img').src="/images/110.png";
      document.getElementById('p1').innerHTML=e.data;
    }
  }

  w.onclose=function(e){
    log('closed');
  }
  window.onload=function(){
    log(log.buffer.join('\n'));
  }
</script>  
</body>
</html>
