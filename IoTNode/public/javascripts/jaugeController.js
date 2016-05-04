/**
 * Created by Guillaume on 04/05/2016.
 */

var receiveData = [];
var socket = io('http://localhost:3000');
socket.on('message', function(data){
    receiveData = data;
    reload();
});

var data;
var chart;

var options = {
    width: 1500, height: 500,
    redFrom: 90, redTo: 100,
    yellowFrom:75, yellowTo: 90,
    minorTicks: 5,
    animation:{
        duration: 500,
        easing: 'inAndOut'
    }

};

google.charts.load('current', {'packages':['gauge']});
google.charts.setOnLoadCallback(drawChart);

function drawChart() {

    data = google.visualization.arrayToDataTable([
        ['Label', 'Value'],
        ['CPU', 0],
        ['Memory', 0],
        ['Disk', 0],
        ['Network', 0]
    ]);

    chart = new google.visualization.Gauge(document.getElementById('chart_div'));

    chart.draw(data, options);
}

function reload(){
    data = google.visualization.arrayToDataTable([
        ['Label', 'Value'],
        ['CPU', receiveData.data[0]],
        ['Memory', receiveData.data[1]],
        ['Disk',receiveData.data[2]],
        ['Network', receiveData.data[3]]
    ]);
    /*data.setValue(0,1, receiveData.data[0]);
    data.setValue(1,1, receiveData.data[1]);
    data.setValue(2,1, receiveData.data[2]);
    data.setValue(3, 1, receiveData.data[3]);*/

    chart = new google.visualization.Gauge(document.getElementById('chart_div'));
    chart.draw(data, options);
}