/**
 * Created by Guillaume on 02/05/2016.
 */



var cpuData = [];
var ramData = [];
var swapData = [];
var diskData = [];
var downloadData = [];
var uploadData = [];

var graphInPcent = [];
var graphTabNetwork = [];

var receiveData = [];
var chart;
var chart2;
var options = {
    title: 'graph %',
    curveType: 'function',
    legend: {position: 'right'}
};

var options2 = {
    title: 'graph network',
    curveType:'function',
    legend: {position:'right'}
};
var baseArray = [];
var cpt = 0;




function drawChartGraph() {
    var dataPcent = google.visualization.arrayToDataTable(graphInPcent);
    var dataNetwork = google.visualization.arrayToDataTable(graphTabNetwork);
    chart = new google.visualization.LineChart(document.getElementById('curve_chart'));
    chart2 = new google.visualization.LineChart(document.getElementById('curve_chart2'));
    chart.draw(dataPcent, options);
    chart2.draw(dataNetwork, options2);
}


function updateDataGraph(values){
    values.forEach(function(data){
        cpuData.push(data.cpu.pcentUsed);
        ramData.push(data.ram.pcentUsed);
        swapData.push(data.swap.pcentUsed);
        diskData.push(data.disk.pcentActive);
        downloadData.push(data.network.debitDown);
        uploadData.push(data.network.debitUp);
    });
    graphTab();
}

function graphTab() {


    graphTabNetwork.push(['i', 'Download', 'Upload']);
    graphInPcent.push(['i', 'CPU', 'RAM', 'SWAP', 'Disk']);
    for (var i = 0; i < cpuData.length; i++) {
        graphInPcent.push([cpt, cpuData[i], ramData[i], swapData[i], diskData[i]]);
        graphTabNetwork.push([cpt, downloadData[i], uploadData[i]]);
        cpt++;
    }
}

function drawGraph(values){
    updateDataGraph(values);
    google.charts.load('current', {'packages': ['corechart']});
    google.charts.setOnLoadCallback(drawChartGraph);
}

if(showGraph && values.length > 0){
    console.log('ok');
    drawGraph(values)
}



