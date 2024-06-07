bool wstringToWchar_T(std::wstring wname1){
	
	
	wchar_t wname2[1024];
	wcscpy(wname2, wname1.c_str());
	wchar_t* pfoo = new wchar_t[1024];
	wcscpy(pfoo, wname1.c_str());
	
	return true;
}
