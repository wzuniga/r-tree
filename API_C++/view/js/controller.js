var fessmodule = angular.module('myModule', []);

fessmodule.controller('ctrlRead', function ($scope, $filter, $http) {
    $scope.canvas = document.getElementById('canvas');
    $scope.ctx = canvas.getContext('2d');

    $scope.onTest = false;

    $scope.items = [];
    $scope.polgItems = [];
    $scope.selectedRegion = "";
    $scope.selectedRegionTemp = "";

    $scope.inputType = "0";

    $scope.memory = [];
    $scope.indexItems = 1;
    
    // mode
    $scope.actionMode = true;
    $scope.classInput = 'btn-default';
    $scope.classSearch = 'btn-info';
    $scope.labelInsert = 'On insert';
    $scope.labelSearch = 'Choose Me';

    //selectQuery
    $scope.queryModel = 'R';
    $scope.kvalue;
    $scope.kdisable = true;

    //general memory
    $scope.regionMemory = [];
    $scope.colorVector = ['#9900ff', '#0099ff', '#00ff33', "#ff8000"];
    $scope.highlightColor = "#ff0000";
    $scope.blackColor = "#000000";
    $scope.highlightNearestColor = "#ff0000";

    //range
    $scope.tempPoint = 0;
    $scope.liveMode = false;

    $scope.count = 0;
    $scope.countR = 0;
    $scope.maxOver = 5;

    $http.post("/rtree/load",{})
        .success(function (data) {
            $scope.render(data);
        })
        .error(function (data) {
            console.log("Time Out " + data);
        }
    );

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
                $scope.drawPoint(point, false, "");
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
                    $scope.drawPoint($scope.polgItems[$scope.polgItems.length-1], false, "");
                }
            }
            $scope.ctx.stroke();
        }else{
            if($scope.queryModel == 'R'){
                if($scope.tempPoint == 0){
                    $scope.tempPoint = {"x":x_c, "y":y_c};
                    $scope.drawPoint($scope.tempPoint, true, $scope.highlightColor);
                }else{
                    var temp = Object.assign({}, $scope.tempPoint);
                    $scope.queryRange(temp, {"x":x_c, "y":y_c});
                    $scope.tempPoint = 0;
                }
            }else{
                $scope.queryKnearest(x_c, y_c, $scope.kvalue);
            }
        }
    }

    $scope.makeRange = function(event){
        var x_c = event.offsetX;
        var y_c = event.offsetY;
        if($scope.queryModel == 'R' && !$scope.actionMode && $scope.liveMode){
            if($scope.tempPoint != 0){
                $scope.clearCanvas();
                if($scope.onTest){
                    var img = new Image();
                    img.src = "/rtree/js/arequipa.jpeg";
                    img.onload = function(){
                        $scope.ctx.drawImage(img, 0, 0);
                        $scope.reDrawCanvas();
                        if($scope.tempPoint != 0){
                            var temp = Object.assign({}, $scope.tempPoint);
                            $scope.drawRange(temp, {"x":x_c, "y":y_c}, $scope.highlightColor);
                            $scope.queryRange(temp, {"x":x_c, "y":y_c});
                        }
                    }
                }else{
                    $scope.reDrawCanvas();
                    if($scope.tempPoint != 0){
                        if ($scope.countR > $scope.maxOver){
                            var temp = Object.assign({}, $scope.tempPoint);
                            $scope.drawRange(temp, {"x":x_c, "y":y_c}, $scope.highlightColor);
                            $scope.queryRange(temp, {"x":x_c, "y":y_c});
                        }else{
                            $scope.countR += 1;
                        }
                    }
                }
            }         
        }else{
            if(!$scope.actionMode && $scope.liveMode)
                if ($scope.count > $scope.maxOver){
                    $scope.queryKnearest(x_c, y_c, $scope.kvalue);
                    $scope.count = 0;
                }else{
                    $scope.count += 1;
                }
                
        }
    };

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
        $scope.reDrawCanvas(); 
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
                "value": "P-"+num+" "+vals,
                "point": item,
                "highlight": false,
            });
        }
        $scope.items.push($scope.memory[$scope.memory.length-1]);
        $scope.indexItems++;
    };

    //delete an item from memory
    $scope.deleteOnMemory = function(index){
        $scope.memory = [];
        $scope.items = [];
        $scope.indexItems = 1;
        $scope.regionMemory = [];  
    };


    /**
     * canvas handler
    **/
    $scope.clearCanvas = function(){
        $scope.ctx.clearRect(0, 0, $scope.canvas.width, $scope.canvas.height);
        $scope.ctx = $scope.canvas.getContext('2d');
    };
    $scope.reDrawCanvas = function(){
        var len = $scope.memory.length;

        for(var i = 0; i < len; i++){
            if($scope.memory[i].type == "P")
                $scope.drawPoint($scope.memory[i].point, $scope.memory[i].highlight, $scope.highlightColor);
            else
                $scope.drawRegion($scope.memory[i].point, $scope.memory[i].highlight, $scope.highlightColor);
        }
        $scope.regionMemory.forEach( function(object, indice, array) {
            $scope.drawRectagle(object[0],object[1],object[2],object[3]);
        });
            

    };


    /**
     * draw functions
    **/
    $scope.drawPoint = function(point, hl, color){
        $scope.ctx.beginPath();
        $scope.ctx.setLineDash([]);
        $scope.ctx.arc(point.x, point.y, 3, 0, 2 * Math.PI);
        if(hl){
            $scope.ctx.lineWidth = 4;
            $scope.ctx.strokeStyle = color;
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
            $scope.ctx.lineWidth = 4;
            $scope.ctx.strokeStyle = color;
        }else{
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = $scope.blackColor;
        }
        for(var i = 0; i < len; i++){
            $scope.ctx.moveTo(arr[i%len].x, arr[i%len].y);
            $scope.ctx.lineTo(arr[(i+1)%len].x, arr[(i+1)%len].y);
        }
        $scope.ctx.stroke();
    };

    $scope.drawRectagle = function(item1, item2, color, text){
        var x = Math.min(item1.x, item2.x);
        var y = Math.min(item1.y, item2.y);
        $scope.ctx.beginPath();
        $scope.ctx.lineWidth = 1;
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
        $scope.ctx.fillText("R"+text,x+1,y+11);
        $scope.ctx.stroke();
    }

    $scope.drawRange = function(item1, item2, color){
        var x = Math.min(item1.x, item2.x);
        var y = Math.min(item1.y, item2.y);
        $scope.ctx.setLineDash([15, 10]);
        $scope.ctx.beginPath();
        $scope.ctx.lineWidth = 2;
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
        $scope.ctx.fillText("Range",x+1,y+11);
        $scope.ctx.stroke();
    }

    $scope.drawLine = function(item1, item2, color){
        $scope.ctx.beginPath();
        $scope.ctx.lineWidth = 2;
        $scope.ctx.strokeStyle = color;
        $scope.ctx.moveTo(item1.x, item1.y);
        $scope.ctx.lineTo(item2.x, item2.y);
        $scope.ctx.stroke();
    }

    $scope.drawGrid = function(){
        for (var i = 0; i < (750); i += 20) {
            $scope.ctx.beginPath();
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = '#ccc';
            $scope.ctx.moveTo(i,0);
            $scope.ctx.lineTo(i, 600);
            $scope.ctx.closePath();
        }
        for (var i = 0; i < (600); i += 20) {
            $scope.ctx.beginPath();
            $scope.ctx.lineWidth = 1;
            $scope.ctx.strokeStyle = '#ccc';
            $scope.ctx.moveTo(0, i);
            $scope.ctx.lineTo(750, i);
            $scope.ctx.closePath();
        }
    };
    /*
    *   Server Manage
    */
    $scope.insertToTree = function(pointArr){
        $http.post("/rtree/insert",{"data":pointArr})
            .success(function (data) {
                $scope.render(data);
            })
            .error(function (data) {
                console.log("Time Out " + data);
            }
        );
    };

    $scope.queryKnearest = function(x_v, y_v, k){
        if(k == undefined || k == 0){
            $scope.kvalue = 1;
            k = 1;
        }
        for(var i=0; i<$scope.selectedRegion.length; i++)
            $scope.selectedRegion[i].highlight = false;
        if($scope.onTest){
            var img = new Image();
            img.src = "/rtree/js/arequipa.jpeg";
            img.onload = function(){
                $scope.ctx.drawImage(img, 0, 0);
            }
        }
        $http.post("/rtree/nearest",{"x":x_v, "y":y_v, "k":k})
            .success(function (data) {
                $scope.clearCanvas();
                if($scope.onTest){
                    var img = new Image();
                    img.src = "/rtree/js/arequipa.jpeg";
                    img.onload = function(){
                        $scope.ctx.drawImage(img, 0, 0);
                        $scope.reDrawCanvas();
                        $scope.dataForNearest(data, x_v, y_v, k);
                    }
                }else{
                    $scope.reDrawCanvas();
                    $scope.dataForNearest(data, x_v, y_v, k);
                }
            })
            .error(function (data) {
                console.log("Time Out " + data);
            }
        );
    };

    $scope.queryRange = function(point_1, point_2){
        if($scope.onTest){
            var img = new Image();
            img.src = "/rtree/js/arequipa.jpeg";
            img.onload = function(){
                $scope.ctx.drawImage(img, 0, 0);
            }
        }
        $http.post("/rtree/range",{"point1":point_1, "point2":point_2})
            .success(function (data) {
                $scope.clearCanvas();
                if($scope.onTest){
                    var img = new Image();
                    img.src = "/rtree/js/arequipa.jpeg";
                    img.onload = function(){
                        $scope.ctx.drawImage(img, 0, 0);
                        $scope.reDrawCanvas();
                        $scope.dataForRange(data, point_1, point_2);
                    }
                }else{
                    $scope.reDrawCanvas();
                    $scope.dataForRange(data, point_1, point_2);
                }
            })
            .error(function (data) {
                console.log("Time Out " + data);
            }
        );
    };

    $scope.euclidean = function(point1, point2){
        return Math.sqrt(Math.pow(point2.x-point1.x,2)+Math.pow(point2.y-point1.y,2));
    };

    $scope.dataFor = function(data){
        data.forEach( function(object, indice, array) {
            if(object.is_leaf == "0"){
                var c_1 = object.elements[0];
                var c_2 = object.elements[1];
                $scope.drawRectagle({"x":c_1[0],"y":c_1[1]},{"x":c_2[0],"y":c_2[1]}, $scope.colorVector[object.level], object.key);
                $scope.regionMemory.push([{"x":c_1[0],"y":c_1[1]},{"x":c_2[0],"y":c_2[1]}, $scope.colorVector[object.level], object.key]);
            }else{
                object.elements.forEach( function(polygon) {
                    if (polygon.length > 1){
                        var temp = [];
                        for(var i=0; i<polygon.length; i++)
                            temp.push({"x":polygon[i][0],"y":polygon[i][1]});
                        $scope.putOnMemory(temp, "R");
                        $scope.drawRegion(temp, false, $scope.blackColor);
                    }else{
                        var c_1 = polygon[0];
                        $scope.putOnMemory({"x":c_1[0],"y":c_1[1]}, "P");
                        $scope.drawPoint({"x":c_1[0],"y":c_1[1]}, false, "");
                    }
                });
            }
        });
    }
    $scope.dataForNearest = function(data, x_v, y_v, k){
        
        data.forEach( function(polygon) {
            if(polygon.length >1){
                var temp = [];
                var min = {"x":polygon[0][0],"y":polygon[0][1]};
                var minDist = $scope.euclidean({"x":polygon[0][0],"y":polygon[0][1]},{"x":x_v, "y":y_v, "k":k});
                for(var i=0; i<polygon.length; i++){
                    temp.push({"x":polygon[i][0],"y":polygon[i][1]});
                    var dist = $scope.euclidean({"x":polygon[i][0],"y":polygon[i][1]},{"x":x_v, "y":y_v, "k":k});
                    if(dist < minDist){
                        minDist = dist;
                        min = {"x":polygon[i][0],"y":polygon[i][1]};
                    }
                }
                $scope.drawRegion(temp, true, $scope.highlightColor);
                $scope.drawLine({"x":x_v, "y":y_v}, min, $scope.highlightNearestColor);
            }else{
                var c_1 = polygon[0];
                $scope.drawPoint({"x":c_1[0],"y":c_1[1]}, true, $scope.highlightColor);
                $scope.drawLine({"x":x_v, "y":y_v}, {"x":c_1[0],"y":c_1[1]}, $scope.highlightNearestColor);
            }
        });
    }

    $scope.dataForRange = function(data, point_1, point_2){
        data.forEach( function(polygon) {
            var points = polygon[0].elements;
            if(points.length >1){
                var temp = [];
                for(var i=0; i<points.length; i++)
                    temp.push({"x":points[i][0],"y":points[i][1]});
                $scope.drawRegion(temp, true, $scope.highlightColor);
            }else{
                var c_1 = points[0];
                $scope.drawPoint({"x":c_1[0],"y":c_1[1]}, true, $scope.highlightColor);
            }
        });
        $scope.drawRange(point_1, point_2, $scope.highlightColor);
    };

    $scope.render = function(data){
        $scope.deleteOnMemory();
        $scope.clearCanvas();
        if($scope.onTest){
            var img = new Image();
            img.src = "/rtree/js/arequipa.jpeg";
            img.onload = function(){
                $scope.ctx.drawImage(img, 0, 0);
                $scope.dataFor(data);
            }
        }else{
            $scope.dataFor(data);
        }
    };

    $scope.clickLoadTest = function(){
        $scope.onTest = true;
        $http.post("/rtree/test",{})
            .success(function (data) {
                $scope.render(data);
            })
            .error(function (data) {
                console.log("Time Out " + data);
            }
        );
    };

    $scope.clickClear = function(){
        $http.post("/rtree/clear",{})
            .success(function (data) {
                $scope.deleteOnMemory();
                $scope.clearCanvas();
                $scope.actionMode = true;
                $scope.queryModel = 'R';
                $scope.inputType = "0";
                $scope.liveMode = false;
                $scope.onTest = false;
            })
            .error(function (data) {
                console.log("Time Out " + data);
            }
        );
    };
});