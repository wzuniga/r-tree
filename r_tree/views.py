from django.shortcuts import render

def getRegionMap(request):
    print(request.method)
    #print(request.data)
    return render(request, 'r_tree/post_list.html', {})
