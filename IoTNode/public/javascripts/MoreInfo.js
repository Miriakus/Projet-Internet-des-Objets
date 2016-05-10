/**
 * Created by Guillaume on 09/05/2016.
 */


function showCPUInfo() {
    $('#cpu').html(`
            user: ${receiveData.data.cpu.user} <br>
            nice: ${receiveData.data.cpu.nice} <br>
            system: ${receiveData.data.cpu.system} <br>
            idle: ${receiveData.data.cpu.idle} <br>
            pcentUsed: ${receiveData.data.cpu.pcentUsed} `);
}

function showRamInfo() {
    $('#ram').html(`
            total: ${receiveData.data.ram.total} <br>
            free: ${receiveData.data.ram.free} <br>
            cached: ${receiveData.data.ram.cached} <br>
            used: ${receiveData.data.ram.used} <br>
            pcentUsed: ${receiveData.data.ram.pcentUsed} `);
}

function showSwapInfo(){
    $('#swap').html(`
        total: ${receiveData.data.swap.total} <br>
        free: ${receiveData.data.swap.free} <br>
        cached: ${receiveData.data.swap.cached} <br>
        used: ${receiveData.data.swap.used} <br>
        pcentUsed: ${receiveData.data.swap.pcentUsed} `);
}

function showDiskInfo(){
    $('#disk').html(`
        totalRead: ${receiveData.data.disk.totalRead} <br>
        totalWrite: ${receiveData.data.disk.totalWrite} <br>
        totalTimeActive: ${receiveData.data.disk.totalTimeActive} <br>
        debitRead: ${receiveData.data.disk.debitRead} <br>
        debitWrite: ${receiveData.data.disk.debitWrite} <br>
        pcentActive: ${receiveData.data.disk.pcentActive} `);
}

function showDownloadInfo(){
    $('#download').html(`
        totalDown: ${receiveData.data.network.totalDown} <br>
        debitDown: ${receiveData.data.network.debitDown} `);
}

function showUploadInfo(){
    $('#upload').html(`
        totalUp: ${receiveData.data.network.totalUp} <br>
        debitUp: ${receiveData.data.network.debitUp} `);
}

$('thead td button').on('click', function () {
    var id = $(this).attr('data-value');
    moreInfo();
    $('#' + id).toggleClass('hide');
});

function moreInfo() {

    setInterval(function(){
        if($('td#cpu').is(':visible')){
            showCPUInfo()
        }
        if($('td#ram').is(':visible')){
            showRamInfo()
        }
        if($('td#swap').is(':visible')){
            showSwapInfo()
        }
        if($('td#disk').is(':visible')){
            showDiskInfo()
        }
        if($('td#download').is(':visible')){
            showDownloadInfo()
        }
        if($('td#upload').is(':visible')){
            showUploadInfo()
        }
    })
}





