var fessmodule = angular.module('myModule', []);

fessmodule.controller('ctrlRead', function ($scope, $filter, $http) {
    $scope.canvas = document.getElementById('canvas');
    $scope.ctx = canvas.getContext('2d');

    $scope.items = [];
    $scope.polgItems = [];
    $scope.selectedRegion = "";
    $scope.selectedRegionTemp = "";

    $scope.inputType = "0";

    $scope.memory = [];
    $scope.indexItems = 1;
    
    // mode
    $scope.actionMode = true;

    $scope.changeState = function(){
        $scope.actionMode = !$scope.actionMode;
    }

    $scope.addOnClick = function(event) {
        console.log("2");
        var x_c = event.offsetX;
        var y_c = event.offsetY;

        if($scope.inputType == "0"){
            var point = {
                "x": x_c,
                "y": y_c,
            };
            $scope.putOnMemory(point, "P");
            $scope.drawPoint(point, false);
            $scope.sendToServer(point);
        }else if($scope.inputType == "1"){
            if($scope.findOnItems($scope.polgItems, x_c, y_c)){
                $scope.putOnMemory($scope.polgItems, "R");
                $scope.drawRegion($scope.polgItems, false);
                $scope.polgItems = [];
            }else{
                $scope.polgItems.push({
                    "x": x_c,
                    "y": y_c,
                });
                $scope.drawPoint($scope.polgItems[$scope.polgItems.length-1], false);
            }
        }
        $scope.ctx.stroke();
    }

    $scope.findOnItems = function(items, x, y) {
        console.log("3");
        for(var i = 0; i < items.length; i++){
            var t_x = items[i].x
            var t_y = items[i].y
            if(t_x+3 >= x && t_x-3 <= x && t_y+3 >= y && t_y-3 <= y)
                return true;
        }
        return false;
    };

    /**
     * highlight Region or Point 
     */
    $scope.updateRegion = function(){
        console.log("4");
        for(var i=0; i<$scope.memory.length; i++)
            $scope.memory[i].highlight = false;
        for(var i=0; i<$scope.selectedRegion.length; i++)
            $scope.selectedRegion[i].highlight = true;
        $scope.clearCanvas();
        $scope.reDrawCanvas();
        console.log($scope.memory);
        console.log($scope.selectedRegion);
        console.log("#");
        $scope.drawRectagle({"x":10,"y":50},{"x":100,"y":100});
    };

    //store on memory an item
    $scope.putOnMemory = function(item, type){
        console.log("5");
        var num = $scope.indexItems;
        if(type == "P"){
            $scope.memory.push({
                "type": "P",
                "index": num,
                "value": "P-"+num+" ("+(item.x)+","+(item.y)+")",
                "point": item,
                "highlight": false,
            });
        }else{
            var vals = "";
            for(var i = 0; i < item.length; i++)
                vals += "("+item[i].x+","+item[i].y+")";

            $scope.memory.push( {
                "type": "R",
                "index": num,
                "value": "R-"+num+" "+vals,
                "point": item,
                "highlight": false,
            });
        }
        $scope.items.push($scope.memory[$scope.memory.length-1]);
        $scope.indexItems++;
    };

    //delete an item from memory
    $scope.deleteOnMemory = function(index){
        console.log("6");
        $scope.clearCanvas();
        $scope.memory.splice(index, 1);
        $scope.reDrawCanvas();    
    };


    /**
     * canvas handler
    **/
    $scope.clearCanvas = function(){
        console.log("7");
        $scope.ctx.clearRect(0, 0, $scope.canvas.width, $scope.canvas.height);
        $scope.ctx = $scope.canvas.getContext('2d');
        //console.log($scope.ctx);
    };
    $scope.reDrawCanvas = function(){
        console.log("8");
        var len = $scope.memory.length;
        for(var i = 0; i < len; i++){
            if($scope.memory[i].type == "P")
                $scope.drawPoint($scope.memory[i].point, $scope.memory[i].highlight);
            else
                $scope.drawRegion($scope.memory[i].point, $scope.memory[i].highlight);
            $scope.ctx.stroke();
        }
    };


    /**
     * draw functions
    **/
    $scope.drawPoint = function(point, hl){
        console.log("9");
        //console.log($scope.ctx);
        $scope.ctx.beginPath();
        $scope.ctx.setLineDash([]);
        console.log(point)
        $scope.ctx.arc(point.x, point.y, 3, 0, 2 * Math.PI);
        if(hl){
            $scope.ctx.lineWidth = 2;
            $scope.ctx.strokeStyle = '#ff0000';
        }else{
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = '#000000';
        }
        $scope.ctx.stroke();
        
    };
    $scope.drawRegion = function(arr, hl){
        console.log("10");
        var len = arr.length;
        $scope.ctx.beginPath();
        $scope.ctx.setLineDash([]);
        if(hl){
            $scope.ctx.lineWidth = 2;
            $scope.ctx.strokeStyle = '#ff0000';
        }else{
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = '#000000';
        }
        for(var i = 0; i < len; i++){
            $scope.ctx.moveTo(arr[i%len].x, arr[i%len].y);
            $scope.ctx.lineTo(arr[(i+1)%len].x, arr[(i+1)%len].y);
        }
        $scope.ctx.stroke();
    };

    $scope.drawRectagle = function(item1, item2){
        $scope.ctx.setLineDash([4, 4]);
        $scope.ctx.beginPath();
        $scope.ctx.strokeStyle = 'blue';
        $scope.ctx.moveTo(item1.x, item1.y);
        $scope.ctx.lineTo(item1.x, item2.y);
        $scope.ctx.moveTo(item1.x, item2.y);
        $scope.ctx.lineTo(item2.x, item2.y);
        $scope.ctx.moveTo(item2.x, item2.y);
        $scope.ctx.lineTo(item2.x, item1.y);
        $scope.ctx.moveTo(item2.x, item1.y);
        $scope.ctx.lineTo(item1.x, item1.y);
        $scope.ctx.stroke();
        //$scope.ctx.strokeRect(cx - 50, cy - 50, 100, 100);
    }


    /*
    *   Server Manage
    */
    $scope.sendToServer = function(point){
        $http.post("/rtree/insert",
            {
                '2':4
            })
            .success(function (data) {
                alert(data);
                //console.log(data);
                //$scope.temp = data;
                //$scope[varb] = data;
                //return data;
            })
            .error(function (data) {
                alert("Error " + data);
            }
        );
    };
});