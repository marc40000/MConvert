@echo off                                       																																				
                                                																																				
for %%i IN (C:\meinedat\projects\cu4\REL\DISTCLIENT\clientdata\cu\Media\buildings2\*.mesh) DO (
	mconvert mesh2ntc %%i
)
                                           																																				
