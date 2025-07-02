SetPosition("0856e7a7-5793-4904-f8bc-8e808686de7c", 5.0, 3.0, -2.0)

local pos = GetPosition("3bd6f09d-204d-b047-359f-6f9194c54263")

if pos ~= nil then
    debugPrint("Posisi: x=" .. pos.x .. ", y=" .. pos.y .. ", z=" .. pos.z)
else
    debugPrint("Object tidak ditemukan")
end