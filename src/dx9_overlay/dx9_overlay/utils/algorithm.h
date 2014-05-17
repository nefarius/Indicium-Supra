#pragma once

template<typename MapContainer, typename Pred>
void erase_if(MapContainer &c, Pred pred)
{
	for (auto p = c.begin(); p != c.end();)
		if (pred(p->first, p->second))
			p = c.erase(p);
		else  ++p;
}
