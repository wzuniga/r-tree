var fessmodule = angular.module('myModule', []);

fessmodule.controller('ctrlRead', function ($scope, $filter, $http) {
    $scope.canvas = document.getElementById('canvas');
    $scope.ctx = canvas.getContext('2d');

    //var img = new Image();
    //img.src = "/rtree/js/arequipa.jpeg";
    //img.onload = function(){
    //    $scope.ctx.drawImage(img, 0, 0);
    //}

    $scope.items = [];
    $scope.polgItems = [];
    $scope.selectedRegion = "";
    $scope.selectedRegionTemp = "";

    $scope.inputType = "0";

    $scope.memory = [];
    $scope.indexItems = 1;
    
    // mode
    $scope.actionMode = true;
    $scope.classInput = 'btn-info';
    $scope.classSearch = 'btn-warning';
    $scope.labelInsert = 'On insert';
    $scope.labelSearch = 'Choose Me';

    //selectQuery
    $scope.queryModel = 'R';
    $scope.kvalue;
    $scope.kdisable = true;

    //general memory
    $scope.regionMemory = [];
    $scope.colorVector = ['#5f5dbb', '#7495bb', '#74c0bb', '#f18539', '#f1c500', '#81c200', '#83b786'];
    $scope.highlightColor = "#ff0000";
    $scope.blackColor = "#000000";

    $scope.changeState = function(){
        $scope.actionMode = !$scope.actionMode;
        var tempClass = $scope.classInput;
        $scope.classInput = $scope.classSearch;
        $scope.classSearch = tempClass;
        var tempLabel = $scope.labelInsert;
        $scope.labelInsert = $scope.labelSearch;
        $scope.labelSearch = tempLabel;
    }

    $scope.addOnClick = function(event) {

        
        var x_c = event.offsetX;
        var y_c = event.offsetY;

        if($scope.actionMode){
            if($scope.inputType == "0"){
                var point = {
                    "x": x_c,
                    "y": y_c,
                };
                $scope.putOnMemory(point, "P");
                $scope.drawPoint(point, false);
                $scope.insertToTree([point]);
            }else if($scope.inputType == "1"){
                if($scope.findOnItems($scope.polgItems, x_c, y_c)){
                    $scope.putOnMemory($scope.polgItems, "R");
                    $scope.drawRegion($scope.polgItems, false, $scope.blackColor);
                    $scope.insertToTree($scope.polgItems);
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
        }else{
            if($scope.queryModel == 'R'){

            }else{
                $scope.queryKnearest(x_c, y_c, $scope.kvalue);
            }
        }
    }

    $scope.findOnItems = function(items, x, y) {
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
        
        for(var i=0; i<$scope.memory.length; i++)
            $scope.memory[i].highlight = false;
        for(var i=0; i<$scope.selectedRegion.length; i++)
            $scope.selectedRegion[i].highlight = true;
        $scope.clearCanvas();
        //var img = new Image();
        //img.src = "/rtree/js/arequipa.jpeg";
        //img.onload = function(){
        //    $scope.ctx.drawImage(img, 0, 0);
        //}
        $scope.reDrawCanvas();
        //$scope.drawRectagle({"x":10,"y":50},{"x":100,"y":100});
        
        
    };

    $scope.currentQueryMode = function(){
        if($scope.queryModel == 'R')
            $scope.kdisable = true;
        else
            $scope.kdisable = false;
    };

    //store on memory an item
    $scope.putOnMemory = function(item, type){
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
        $scope.clearCanvas();
        $scope.memory.splice(index, 1);
        $scope.reDrawCanvas();    
    };


    /**
     * canvas handler
    **/
    $scope.clearCanvas = function(){
        $scope.ctx.clearRect(0, 0, $scope.canvas.width, $scope.canvas.height);
        $scope.ctx = $scope.canvas.getContext('2d');
        //console.log($scope.ctx);
    };
    $scope.reDrawCanvas = function(){
        var len = $scope.memory.length;
        for(var i = 0; i < len; i++){
            if($scope.memory[i].type == "P")
                $scope.drawPoint($scope.memory[i].point, $scope.memory[i].highlight);
            else
                $scope.drawRegion($scope.memory[i].point, $scope.memory[i].highlight, $scope.blackColor);
            //$scope.ctx.stroke();
        }
        $scope.regionMemory.forEach( function(object, indice, array) {
            $scope.drawRectagle(object[0],object[1],object[2]);
        });
    };


    /**
     * draw functions
    **/
    $scope.drawPoint = function(point, hl){
        //console.log($scope.ctx);
        $scope.ctx.beginPath();
        $scope.ctx.setLineDash([]);
        $scope.ctx.arc(point.x, point.y, 3, 0, 2 * Math.PI);
        if(hl){
            $scope.ctx.lineWidth = 2;
            $scope.ctx.strokeStyle = $scope.highlightColor;
        }else{
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = '#000000';
        }
        $scope.ctx.stroke();
        
    };
    $scope.drawRegion = function(arr, hl, color){
        var len = arr.length;
        $scope.ctx.beginPath();
        $scope.ctx.setLineDash([]);
        if(hl){
            $scope.ctx.lineWidth = 2;
            $scope.ctx.strokeStyle = $scope.highlightColor;
        }else{
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = color;
        }
        for(var i = 0; i < len; i++){
            $scope.ctx.moveTo(arr[i%len].x, arr[i%len].y);
            $scope.ctx.lineTo(arr[(i+1)%len].x, arr[(i+1)%len].y);
        }
        $scope.ctx.stroke();
    };

    $scope.drawRectagle = function(item1, item2, color){
        var x = Math.min(item1.x, item2.x);
        var y = Math.min(item1.y, item2.y);
        $scope.ctx.setLineDash([4, 4]);
        $scope.ctx.beginPath();
        $scope.ctx.strokeStyle = color;
        $scope.ctx.moveTo(item1.x, item1.y);
        $scope.ctx.lineTo(item1.x, item2.y);
        $scope.ctx.moveTo(item1.x, item2.y);
        $scope.ctx.lineTo(item2.x, item2.y);
        $scope.ctx.moveTo(item2.x, item2.y);
        $scope.ctx.lineTo(item2.x, item1.y);
        $scope.ctx.moveTo(item2.x, item1.y);
        $scope.ctx.lineTo(item1.x, item1.y);
        $scope.ctx.font = "13px Arial";
        $scope.ctx.fillText("R1",x+1,y+11);
        $scope.ctx.stroke();
        //$scope.ctx.strokeRect(cx - 50, cy - 50, 100, 100);
    }


    /*
    *   Server Manage
    */
    $scope.insertToTree = function(pointArr){
        $http.post("/rtree/insert",{"data":pointArr})
            .success(function (data) {
                console.log(data);
                $scope.clearCanvas();
                //$scope.reDrawCanvas();
                data.forEach( function(object, indice, array) {
                    //console.log("En el índice " + indice + " hay este valor: " + object);
                    if(object.is_leaf == "0"){
                        var c_1 = object.elements[0];
                        var c_2 = object.elements[1];
                        $scope.drawRectagle({"x":c_1[0],"y":c_1[1]},{"x":c_2[0],"y":c_2[1]}, $scope.colorVector[object.level]);
                        $scope.regionMemory.push([{"x":c_1[0],"y":c_1[1]},{"x":c_2[0],"y":c_2[1]}, $scope.colorVector[object.level]]);
                    }else{
                        object.elements.forEach( function(polygon) {
                            console.log("##");
                            console.log(polygon);
                            if (polygon.length > 1){
                                var temp = [];
                                for(var i=0; i<polygon.length; i++)
                                    temp.push({"x":polygon[i][0],"y":polygon[i][1]});
                                $scope.drawRegion(temp, false, $scope.blackColor);
                            }else{
                                var c_1 = polygon[0];
                                console.log(c_1[0]);
                                $scope.drawPoint({"x":c_1[0],"y":c_1[1]}, false);
                            }
                        });
                    }
                });
            })
            .error(function (data) {
                alert("Error " + data);
            }
        );
    };

    $scope.queryKnearest = function(x_v, y_v, k){
        if(k == undefined || k == 0){
            $scope.kvalue = 1;
            k = 1;
        }
        $http.post("/rtree/nearest",{"x":x_v, "y":y_v, "k":k})
            .success(function (data) {
                console.log(data);

            })
            .error(function (data) {
                alert("Error " + data);
            }
        );
    };

    

});