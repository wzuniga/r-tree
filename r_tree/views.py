from django.shortcuts import render

def post_list(request):
    return render(request, 'r_tree/post_list.html', {})
