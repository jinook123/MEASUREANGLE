<?
include 'index.php';

$sql='select date(time) as time,avg(sit_time) as sit_time from sit_time group by date(time) ';

$result=mysqli_query($mysqli,$sql);

$total_num=mysqli_num_rows($result);

for($i=0;$i<$total_num;$i++){
   mysqli_data_seek($result,$i);
   $row=mysqli_fetch_array($result);

   $times[]=$row['time'];
   $sit_times[]=round($row['sit_time']);
}

//var_dump($times);


include 'index.php';
$sql3 = 'select date(time) as time, round(avg(angle)) as angle from angle where time between(now() -interval 15 day) and (now() - interval 9 day) group by date(time)';

$result3 = mysqli_query($mysqli,$sql3);
$total_num3 = mysqli_num_rows($result3);

for($k=0;$k<$total_num3;$k++){
   mysqli_data_seek($result3,$k);
   $row=mysqli_fetch_array($result3);

   $time2[]=$row['time'];
   $angle2[]=$row['angle'];



     if($angle2[$k]<=125){

         $x2[$k] =  round((125- $angle2[$k])/110 * 100);

         $x3[$k] = 100 - $x2[$k];

   

      } else {

         $x2[$k] =  round((125 - $angle2[$k])/110 * 100);

         $x3[$k] = 100 + $x2[$k];

      
      }



   //echo "$time2[4]";
   //echo"\n";
   //echo "$x3[4]";
   //echo "\n";

   }


include 'index.php';
 $sql4 = 'select dayname(time) as time, round(avg(angle)) as angle from angle where time between(now() -interval 8 day) and (now() - interval 1 day) group by date(time)';
$result4 = mysqli_query($mysqli,$sql4);
$total_num4 = mysqli_num_rows($result4);

for($m=0;$m<$total_num4;$m++){
   mysqli_data_seek($result4,$m);
   $row=mysqli_fetch_array($result4);

   $time3[]=$row['time'];
   $angle3[]=$row['angle'];



if($angle3[$m]<=125){

         $x3[$m] =  round((125- $angle3[$m])/110 * 100);

         $x4[$m] = 100 - $x3[$m];

   
      } else {

         $x3[$m] =  round((125 - $angle3[$m])/110 * 100);

         $x4[$m] = 100 + $x3[$m];

   

   

   }
   //echo "$time3[0]";
   //echo"\n";
   //echo "$x4[0]";
   //echo "\n";

   }





?>






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
              width : 300px;
              height: 170px;
              float : center;
              margin-top: 15px;
              /*margin-right: px;*/
              border: 2px double;
              border-color:rgba(76,185,192,0.5);
            
           }


           canvas{
            margin-top: 7px;
           }

          

            .line2{
                 /* margin:50px auto;*/
                  width: 300px;
                  height: 170px;
                  float:center;
                  margin-bottom: 15px;
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


     </style>


       </head>

      


    <body>

     <center>

       <div id ="main" style="
         /*padding-top:50px; */
         margin-top: 30px;
         border : 5px double #4CB9C0;
         height: 400px;
         width: 330px;
        ">

      <center>
      <!--<div class="graph" style="float:center;" >-->
         <div class="line">
            <canvas id="line-bar" 
           width="300" height="150">
             </canvas>
         </div>

         <br>
         

         <div class="line2">
         
            <canvas id="canvas" 
            width = "300" height="160">
            </canvas>

         </div>

      <!--</div>-->
      </center>

<script>
   var times=<?php echo json_encode($times);?>;
   var sit_times=<?php echo json_encode($sit_times);?>;
   //alert(times);
   //alert(sit_times);
   var time2=<?php echo json_encode($time2);?>;
   var angle3=<?php echo json_encode($x3);?>;

   var time3=<?php echo json_encode($time3);?>;
   var angle4=<?php echo json_encode($x4);?>;



    var randomScalingFactor = function(){ return Math.round(Math.random()*100)};
    window.onload = function(){
    var ctx = document.getElementById("line-bar").getContext("2d");
        window.myLineBarChart = new Chart(ctx).LineBar({

        labels : [times[0],times[1],times[2],times[3],times[4],times[5],times[6]],
        datasets: [{
        label: "grade average",
        //new option, type will default to bar as that what is used to create the scale
        type: "line",
        fillColor: "rgba(255,255,255,0)",
        strokeColor: "rgba(203,67,53,1)",
        pointColor: "rgba(203,67,53,1)",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "rgba(203,67,53,0)",
        data: [sit_times[0],sit_times[1],sit_times[2],sit_times[3],sit_times[4],sit_times[5],sit_times[6]]
    }, {
        label: "grade",
        //new option, type will default to bar as that what is used to create the scale
        type: "bar",
                fillColor: "rgba(65,86,164,0.5)",
                strokeColor: "rgba(36,42,106,0.1)",
                pointColor: "rgba(65,86,164,0.3)",
                pointStrokeColor: "#fff",
                pointHighlightFill: "#fff",
                pointHighlightStroke: "rgba(151,187,205,1)",
        data:[sit_times[0],sit_times[1],sit_times[2],sit_times[3],sit_times[4],sit_times[5],sit_times[6]]
        }]
    }, {scaleOverride: true, scaleStartValue: 0, scaleStepWidth: 10, scaleSteps: 9,scaleFontColor: "gray",scaleFontSize: 7});


    var lineChartData = {
        labels: [time3[0],time3[1],time3[2],time3[3],time3[4],time3[5],time3[6]],
        datasets: [
            {
                label: "last week",
               fillColor : "rgba(220,220,220,0.2)",
            strokeColor : "rgba(220,220,220,1)",
            pointColor : "rgba(220,220,220,1)",
            pointStrokeColor : "#fff",
            pointHighlightFill : "#fff",
            pointHighlightStroke : "rgba(220,220,220,1)",
                data: [angle3[0],angle3[1],angle3[2],angle3[3],angle3[4],angle3[5],angle3[6]]
            },
            {
                label: "this week",
                fillColor : "rgba(151,187,205,0.2)",
            strokeColor : "rgba(151,187,205,1)",
            pointColor : "rgba(151,187,205,1)",
            pointStrokeColor : "#fff",
            pointHighlightFill : "#fff",
            pointHighlightStroke : "rgba(151,187,205,1)",
                data: [angle4[0],angle4[1],angle4[2],angle4[3],angle4[4],angle4[5],angle4[6]]
            }
        ]
    }
      var ctx = document.getElementById("canvas").getContext("2d");
      window.myLine = new Chart(ctx).Line(lineChartData, {
         responsive: true
        });


}
    </script>

      
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

</center>      
</body>
</html>