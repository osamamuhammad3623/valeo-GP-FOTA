from django.shortcuts import render
import firebase_interface

def landing_page(request):
    return render(request, 'home_page.html')