/**
 * Created by Guillaume on 02/05/2016.
 */

var cpuDatas = [0,4,0,0,3,10,45,20,3,2,2,1,4,6,2];
var ramDatas = [48,48,48,48,48,48,48,48,48,48,48,48,48,48,60];
var diskDatas = [0,0,0,0,0,0,4,50,20,10,3, 0,0,0,0];
var linkDatas = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];

function updateDatasGraph(newCpuValue, newRamValue, newDiskValue, newLinkValue) {
    if (cpuDatas.length >= 15) {
        delete cpuDatas[0];
        cpuDatas.push(newCpuValue);

        delete ramDatas[0];
        ramDatas.push(newRamValue);

        delete diskDatas[0];
        diskDatas.push(newDiskValue);

        delete linkDatas[0];
        linkDatas.push(newLinkValue);
    }
    else {
        cpuDatas.push(newCpuValue);
        ramDatas.push(newRamValue);
        diskDatas.push(newDiskValue);
        linkDatas.push(newLinkValue);
    }
}

function graphTab(){
    var graphTab = [];
    for(var i = 0; i < cpuDatas.length; i ++){
        graphTab.push([i , cpuDatas[i], ramDatas[i], diskDatas[i], linkDatas[i]])
    }
    return graphTab;
}
