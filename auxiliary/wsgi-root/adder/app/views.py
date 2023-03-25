from django.shortcuts import render


# Create your views here.

def app(request):
    return render(request, 'app.html')


def add(request):
    x = request.GET.get('x')
    y = request.GET.get('y')
    sum = int(x) + int(y)
    return render(request, 'app.html', {'sum': sum})
